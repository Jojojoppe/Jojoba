#include "asset_manager.hpp"
#include "stringutils.hpp"

#include "material/material_phong.hpp"

#include <cstdio>
#include <stdexcept>
#include <fstream>
#include <cstring>

#ifdef _WIN32
    #define PLATFORM_PATH_DELIMITER '\\'
#else
    #define PLATFORM_PATH_DELIMITER '/'
#endif

Jojoba::Core::AssetManager::AssetManager(std::shared_ptr<VmInstance> vmInstance){
    this->vmInstance = vmInstance;
}

Jojoba::Core::AssetManager::~AssetManager(){
    for(auto& i : textures) textures.erase(i.first);
    for(auto& i : samplers) samplers.erase(i.first);
    for(auto& i : objects) objects.erase(i.first);
}

bool Jojoba::Core::AssetManager::loadObject(const std::string& path, const std::string& name){
    // Expand path
    std::vector<std::string> path_expanded;
    string_split(path_expanded, path, PLATFORM_PATH_DELIMITER);

    // Get object type
    std::vector<std::string> object_name;
    string_split(object_name, path_expanded.back(), '.');
    if(object_name.back()=="obj"){
        return __loadObjFile(path_expanded, name);
    }else{
        vmInstance->logger.log(VMCORE_LOGLEVEL_ERROR, "Jojoba::Core::AssetManager::loadObject() - Unknown filetype '%s'", object_name.back().c_str());
        return false;
    }

    return false;
}

// OBJ files contain PHONG materials
// TODO USE PLATFORM PATH DELIMITER
bool Jojoba::Core::AssetManager::__loadObjFile(const std::vector<std::string>& path_expanded, const std::string& name){
    std::string directory = "";
    std::string filename = path_expanded.back();
    for(int i=0; i<path_expanded.size()-1; i++){
        directory += path_expanded[i] + "/";
    }
    vmInstance->logger.log(VMCORE_LOGLEVEL_DEBUG, "Loading %s%s", directory.c_str(), filename.c_str());

    std::shared_ptr<Object> o = nullptr;
    std::shared_ptr<Mesh> m = nullptr;
    unsigned int mesh_offset = 0;
    unsigned int mesh_size = 0;

    Object * newobj = new Object;
    newobj->name = name;
    // TODO check if already in map
    objects.insert(std::pair<std::string, std::shared_ptr<Object>>(newobj->name, std::shared_ptr<Object>(newobj)));
    o = objects[newobj->name];

    struct vertex{
        float x, y, z, w=1.0;
    };
    std::vector<vertex> vertices = {vertex{}};
    struct uv{
        float u, v=0.0, w=0.0;
    };
    std::vector<uv> uvs = {uv{}};
    struct normal{
        float i, j, k;  
    };
    std::vector<normal> normals = {normal{}};
    struct face{
        unsigned int v1, v2, v3;
        unsigned int t1, t2, t3;
        unsigned int n1, n2, n3;
    };
    std::vector<face> faces;

    std::ifstream file(directory+"/"+filename);
    char buf[512];
    while(!file.eof()){
        file.getline(buf, 512);

        // Ignore comments
        if(buf[0]=='#') continue;

        // mtllib definition
        if(memcmp(buf, "mtllib", 6)==0){
            std::string mtllibfilename(buf+strlen("mtllib "));
            std::vector<std::string> mtlfile(path_expanded);
            mtlfile.back() = mtllibfilename;
            __loadMtlFile(mtlfile);
        }

        // Start of new object
        if(memcmp(buf, "o ", 2)==0){
            // NOTHING TO DO
        }

        // Vertex definition
        if(memcmp(buf, "v ", 2)==0){
            vertex v;
            sscanf(buf, "v %f %f %f %f", &v.x, &v.y, &v.z, &v.w);
            vertices.push_back(v);
        }

        // UV definition
        if(memcmp(buf, "vt", 2)==0){
            uv u;
            sscanf(buf, "vt %f %f %f", &u.u, &u.v, &u.w);
            uvs.push_back(u);
        }

        // Normal definition
        if(memcmp(buf, "vn", 2)==0){
            normal n;
            sscanf(buf, "vn %f %f %f", &n.i, &n.j, &n.k);
            normals.push_back(n);
        }

        // Material usage definition -> new mesh
        if(memcmp(buf, "usemtl", 6)==0){
            if(m){
                // Set offset and length of mesh
                m->offset = mesh_offset;
                m->length = mesh_size;
                mesh_offset += mesh_size;
                mesh_size = 0;
            }
            Mesh * newm = new Mesh;
            o->meshes.push_back(std::shared_ptr<Mesh>(newm));
            m = o->meshes.back();
            m->material = std::string(buf+strlen("usemtl "));
        }

        // Face definition
        if(memcmp(buf, "f ", 2)==0){
            std::vector<std::string> verts;
            string_split(verts, std::string(buf+strlen("f ")), ' ');
            if(verts.size()<3) continue;        // Face must be at least a triangle
            // triangulate face
            int last = 1;
            for(int i=2; i<verts.size(); i++){
                face f = {};
                sscanf(verts[0].c_str(), "%u/%u/%u", &f.v1, &f.t1, &f.n1);
                sscanf(verts[last].c_str(), "%u/%u/%u", &f.v2, &f.t2, &f.n2);
                sscanf(verts[last+1].c_str(), "%u/%u/%u", &f.v3, &f.t3, &f.n3);
                last = last+1;

                faces.push_back(f);
                mesh_size++;
            }
        }
    }

    // Update last mesh
    m->offset = mesh_offset;
    m->length = mesh_size;

    // Create vertex and index buffer
    std::vector<float> vdata;
    std::vector<unsigned int> idata;

    unsigned int index = 0;
    for(unsigned int i=0; i<faces.size(); i++){
        vdata.insert(vdata.end(), {vertices[faces[i].v1].x, vertices[faces[i].v1].y, vertices[faces[i].v1].z, vertices[faces[i].v1].w});
        if(faces[i].t1)
            vdata.insert(vdata.end(), {uvs[faces[i].t1].u, uvs[faces[i].t1].v, uvs[faces[i].t1].w});
        else
            vdata.insert(vdata.end(), {0,0,0});
        if(faces[i].n1)
            vdata.insert(vdata.end(), {normals[faces[i].n1].i, normals[faces[i].n1].j, normals[faces[i].n1].k});
        else
            vdata.insert(vdata.end(), {0,0,0});

        vdata.insert(vdata.end(), {vertices[faces[i].v2].x, vertices[faces[i].v2].y, vertices[faces[i].v2].z, vertices[faces[i].v2].w});
        if(faces[i].t2)
            vdata.insert(vdata.end(), {uvs[faces[i].t2].u, uvs[faces[i].t2].v, uvs[faces[i].t2].w});
        else
            vdata.insert(vdata.end(), {0,0,0});
        if(faces[i].n2)
            vdata.insert(vdata.end(), {normals[faces[i].n2].i, normals[faces[i].n2].j, normals[faces[i].n2].k});
        else
            vdata.insert(vdata.end(), {0,0,0});

        vdata.insert(vdata.end(), {vertices[faces[i].v3].x, vertices[faces[i].v3].y, vertices[faces[i].v3].z, vertices[faces[i].v3].w});
        if(faces[i].t3)
            vdata.insert(vdata.end(), {uvs[faces[i].t3].u, uvs[faces[i].t3].v, uvs[faces[i].t3].w});
        else
            vdata.insert(vdata.end(), {0,0,0});
        if(faces[i].n3)
            vdata.insert(vdata.end(), {normals[faces[i].n3].i, normals[faces[i].n3].j, normals[faces[i].n3].k});
        else
            vdata.insert(vdata.end(), {0,0,0});

        idata.insert(idata.end(), {index, index+1, index+2});
        index+=3;
    }

    vmInstance->createBuffer(o->vertex_buffer, vdata.size()*sizeof(float), Vermilion::Core::BufferType::BUFFER_TYPE_VERTEX, Vermilion::Core::BufferUsage::BUFFER_USAGE_WRITE_ONLY, Vermilion::Core::BufferDataUsage::BUFFER_DATA_USAGE_STATIC);
    vmInstance->createBuffer(o->index_buffer, idata.size()*sizeof(unsigned int), Vermilion::Core::BufferType::BUFFER_TYPE_INDEX, Vermilion::Core::BufferUsage::BUFFER_USAGE_WRITE_ONLY, Vermilion::Core::BufferDataUsage::BUFFER_DATA_USAGE_STATIC);
    o->vertex_buffer.setData(vdata.data());
    o->index_buffer.setData(idata.data());

    // Create renderables and bindings
    for(auto& m : o->meshes){
        vmInstance->createRenderable(m->mesh, o->vertex_buffer, o->index_buffer, 0, m->offset*3, m->length*3);
        std::shared_ptr<MaterialPhong> mat = std::static_pointer_cast<MaterialPhong>(materials[m->material]);
        vmInstance->createBinding(m->binding, {}, {
            // map_ambient
            samplers[mat->map_diffuse].get()
            // map_specular
            // map_emission
            // map_normal
        });
    }

    return true;
}

// MTL files contain PHONG materials
// TODO USE PLATFORM PATH DELIMITER
bool Jojoba::Core::AssetManager::__loadMtlFile(const std::vector<std::string>& path_expanded){
    std::string directory = "";
    std::string filename = path_expanded.back();
    for(int i=0; i<path_expanded.size()-1; i++){
        directory += path_expanded[i] + "/";
    }
    vmInstance->logger.log(VMCORE_LOGLEVEL_DEBUG, "Loading %s%s", directory.c_str(), filename.c_str());

    std::shared_ptr<MaterialPhong> m = nullptr;

    std::ifstream file(directory+"/"+filename);
    char buf[512];
    while(!file.eof()){
        file.getline(buf, 512);

        // Ignore comments
        if(buf[0]=='#') continue;

        // Start of new material definition
        if(memcmp(buf, "newmtl", 6)==0){
            MaterialPhong * newmat = new MaterialPhong;
            newmat->name = std::string(buf+strlen("newmtl "));
            // TODO check if already in map
            materials.insert(std::pair<std::string, std::shared_ptr<Material>>(newmat->name, std::shared_ptr<Material>(newmat)));
            m = std::static_pointer_cast<MaterialPhong>(materials[newmat->name]);
        }

        // Ambient, diffuse, specular and emission colors
        if(memcmp(buf, "Ka", 2)==0) sscanf(buf, "Ka %f %f %f", &m->ambient.r, &m->ambient.g, &m->ambient.b);
        if(memcmp(buf, "Kd", 2)==0) sscanf(buf, "Kd %f %f %f", &m->diffuse.r, &m->diffuse.g, &m->diffuse.b);
        if(memcmp(buf, "Ks", 2)==0) sscanf(buf, "Ks %f %f %f", &m->specular.r, &m->specular.g, &m->specular.b);
        if(memcmp(buf, "Ke", 2)==0) sscanf(buf, "Ke %f %f %f", &m->emission.r, &m->emission.g, &m->emission.b);

        // Ambient, diffuse, specular and emission maps
        if(memcmp(buf, "map_Ka", 6)==0){
            m->map_ambient = std::string(buf + strlen("map_Ka ")); 
            __loadTexture(directory, m->map_ambient);
        }
        if(memcmp(buf, "map_Kd", 6)==0){
            m->map_diffuse = std::string(buf + strlen("map_Kd ")); 
            __loadTexture(directory, m->map_diffuse);
        }
        if(memcmp(buf, "map_Ks", 6)==0){
            m->map_specular = std::string(buf + strlen("map_Ks ")); 
            __loadTexture(directory, m->map_specular);
        }
        if(memcmp(buf, "map_Ke", 6)==0){
            m->map_emission = std::string(buf + strlen("map_Ke ")); 
            __loadTexture(directory, m->map_emission);
        }
        if(memcmp(buf, "map_bump", 8)==0){
            m->map_normal = std::string(buf + strlen("map_bump ")); 
            __loadTexture(directory, m->map_normal);
        }

        // Specular exponent, optical density and transparance
        if(memcmp(buf, "Ns", 2)) sscanf(buf, "Ns %f", &m->specular_exponent);
        if(memcmp(buf, "Ni", 2)) sscanf(buf, "Ni %f", &m->optical_density);
        if(memcmp(buf, "d ", 2)) sscanf(buf, "d %f", &m->transparance);

    }

    return true;
}

// TODO USE PLATFORM PATH DELIMITER
bool Jojoba::Core::AssetManager::__loadTexture(const std::string& directory, const std::string filename){
    if(textures.find(filename)==textures.end()){
        // Not yet in asset manager, create texture
        vmInstance->logger.log(VMCORE_LOGLEVEL_DEBUG, "Loading %s%s", directory.c_str(), filename.c_str());

        textures.insert(std::pair<std::string, std::shared_ptr<VmTexture>>(filename, std::shared_ptr<VmTexture>(new VmTexture)));
        samplers.insert(std::pair<std::string, std::shared_ptr<VmSampler>>(filename, std::shared_ptr<VmSampler>(new VmSampler)));

        size_t width, height, channels;
        unsigned char * pixels = Vermilion::Core::loadTextureData(directory+"/"+filename, &width, &height, &channels);
        vmInstance->createTexture(*textures[filename], width, height, channels);
        textures[filename]->setData(pixels);
        Vermilion::Core::freeTextureData(pixels);

        vmInstance->createSampler(*samplers[filename], *textures[filename]);
    }
    return true;
}