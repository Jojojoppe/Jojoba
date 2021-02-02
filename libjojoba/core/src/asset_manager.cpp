#include "asset_manager.hpp"
#include "stringutils.hpp"
#include <fstream>
#include <cstring>

#include "materials/no_material.hpp"

#ifdef _WIN32
    #define PLATFORM_PATH_DELIMITER '\\'
#else
    #define PLATFORM_PATH_DELIMITER '/'
#endif

Jojoba::Core::AssetManager::AssetManager(std::shared_ptr<VmInstance> vmInstance){
    this->vmInstance = vmInstance;

    // Initialize material types
    assetMaterialTypes.insert(std::pair<unsigned int, AssetMaterialType>(IDcounter, AssetMaterialType{}));
    assetMaterialTypes[IDcounter++].material = Jojoba::Core::NoMaterial::init(vmInstance);
}

Jojoba::Core::AssetManager::~AssetManager(){
}

unsigned int Jojoba::Core::AssetManager::loadObjectFromObjFile(const std::string& path){
    // Expand path
    std::vector<std::string> path_expanded;
    string_split(path_expanded, path, PLATFORM_PATH_DELIMITER);

    std::string directory = "";
    std::string filename = path_expanded.back();
    for(int i=0; i<path_expanded.size()-1; i++){
        directory += path_expanded[i] + PLATFORM_PATH_DELIMITER;
    }
    vmInstance->logger.log(VMCORE_LOGLEVEL_DEBUG, "Loading %s%s", directory.c_str(), filename.c_str());
    
    unsigned int mesh_offset = 0;
    unsigned int mesh_size = 0;

    assetObjects.insert(std::pair<unsigned int, AssetObject>(IDcounter, AssetObject{}));
    unsigned int objectID = IDcounter;
    AssetObject * o = &assetObjects[IDcounter++];

    struct mesh{
        unsigned int offset, length;
    };
    std::vector<mesh> addedMeshes;
    mesh * m = nullptr;
    
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
            // __loadMtlFile(mtlfile);
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
                // Create renderable for mesh
                m->length = mesh_size;
                m->offset = mesh_offset;
                mesh_offset += mesh_size;
                mesh_size = 0;
            }
            addedMeshes.push_back(mesh{});
            m = &addedMeshes.back();
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
    
    vmInstance->createBuffer(o->vBuffer, vdata.size()*sizeof(float), Vermilion::Core::BufferType::BUFFER_TYPE_VERTEX, Vermilion::Core::BufferUsage::BUFFER_USAGE_WRITE_ONLY, Vermilion::Core::BufferDataUsage::BUFFER_DATA_USAGE_STATIC);
    vmInstance->createBuffer(o->iBuffer, idata.size()*sizeof(unsigned int), Vermilion::Core::BufferType::BUFFER_TYPE_INDEX, Vermilion::Core::BufferUsage::BUFFER_USAGE_WRITE_ONLY, Vermilion::Core::BufferDataUsage::BUFFER_DATA_USAGE_STATIC);
    o->vBuffer.setData(vdata.data());
    o->iBuffer.setData(idata.data());    

    // Create AssetMesh objects
    for(const auto& m : addedMeshes){
        assetMeshes.insert(std::pair<unsigned int, AssetMesh>(IDcounter, AssetMesh{}));
        o->meshes.push_back(IDcounter);
        AssetMesh * nm = &assetMeshes[IDcounter++];
        vmInstance->createRenderable(nm->renderable, o->vBuffer, o->iBuffer, 0, m.offset*3, m.length*3);
    }

    return objectID;
}

unsigned int Jojoba::Core::AssetManager::createScene(){
    assetScenes.insert(std::pair<unsigned int, AssetScene>(IDcounter, AssetScene{}));
    assetScenes[IDcounter].scene = new Jojoba::Core::Scene();
    unsigned int s = IDcounter;
    IDcounter++;
    return IDcounter;
}
