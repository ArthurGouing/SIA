import bpy
from time import time

def export_anim():
    # Open the files
    file_name = "test.bvh"
    file = "/Users/gouinguenet/"+file_name
    bpy.ops.export_anim.bvh(filepath=file,
                            frame_start=0,
                            frame_end=100)
    
def export_weight():
    """ Export the Weight in custom format """
    # Select object to export
    obj  = bpy.context.view_layer.objects.active.children[0]
    vertices = obj.to_mesh().vertices[:]
    Vgroups = obj.vertex_groups[:]
        
    # Open the files
    file_name = "test"
    file = "/Users/gouinguenet/"+file_name
    weight_file = open(file + ".wgt", "w")
    
    # Write header
    weight_file.write(" ".join([g.name for g in Vgroups]))
    weight_file.write("\n")
    
    # Write all vertices
    for v in vertices:
        # Weight
        Weight = list()
        for g in Vgroups:
            try :
                w = g.weight(v.index)
            except RuntimeError: # If the werices isn't in the group
                w = 0
            Weight.append(f"{w:.8f}")
        weight_file.write(" ".join(Weight)+"\n")
        
    # Close files
    weight_file.close()


    
def export_mesh():
    """ Export the mesh in OFF format """
    # Select object to export
    obj  = bpy.context.view_layer.objects.active.children[0]
    mesh = obj.to_mesh()
    print(obj.name)
    # Select vert and face
    vertices   = mesh.vertices[:]
    n_vertices = len(vertices)
    faces   = mesh.polygons[:]
    n_faces = len(faces)
    
    # Open the files
    file_name = "test"
    file = "/Users/gouinguenet/"+file_name
    geom_file = open(file + ".off", "w")
    
    # Write Headers
    geom_file.write("OFF\n")
    geom_file.write(f"{n_vertices} {n_faces} 0\n")
    
    # Write all vertices
    for v in vertices:
        index = v.index
        # Geometry
        loc_x = v.co[0]
        loc_y = v.co[1]
        loc_z = v.co[2]
        geom_file.write(f"{loc_x:.8f} {loc_y:.8f} {loc_z:.8f}\n")

    # Write all faces
    for f in faces:
        geom_file.write(f"{len(f.vertices)} ")
        # print([id for id in f.vertices])
        geom_file.write(" ".join([str(id) for id in f.vertices]))
        geom_file.write("\n")
    
    
    # Close files
    geom_file.close()

    
print("---------------------------------------------\n")
print("Start:")
t = time()
export_mesh()
print(f"The export mesh took {time()-t:.2f} s")
t = time()
export_weight()
print(f"The export weights took {time()-t:.2f} s")
t = time()
export_anim()
print(f"The export mesh took {time()-t:.2f} s")
print("")
