import bpy
import mathutils


def is_inside(p, max_dist, obj):

    # max_dist = 1.84467e+19

    point, normal, face = obj.closest_point_on_mesh(p, max_dist)

    p2 = point-p

    v = p2.dot(normal)
    return not(v < 0.0)




grafo = bpy.data.objects["grafo"]
mesh = grafo.data;
level = bpy.data.objects["level"]
levelName = level.data.name

f = open('/home/flush/CEDV/pacman/pacman/blender/'+levelName+'.xml','w')
f.write('<?xml version="1.0" encoding="UTF-8"?>\n')
f.write('<graphml xmlns="http://graphml.graphdrawing.org/xmlns"\n')  
f.write('xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"\n')
f.write('xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns\n')
f.write('http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">\n')
f.write('<graph id="'+levelName+'" edgedefault="undirected" >\n')
for idx,ver in enumerate(mesh.vertices):
    if not(is_inside(mathutils.Vector((ver.co.x,ver.co.y,ver.co.z)),1,level)):
        f.write('<node id="'+str(idx)+'" >\n')
        f.write('<data id="x">'+str(ver.co.x)+'</data>\n')
        f.write('<data id="y">'+str(ver.co.z)+'</data>\n')
        f.write('<data id="z">'+str(ver.co.y)+'</data>\n')
        f.write('</node>\n')
for idxe, edge in enumerate(mesh.edges):
    vertex1 = mesh.vertices[edge.vertices[0]]
    vertex2 = mesh.vertices[edge.vertices[1]]
    if(not(is_inside(mathutils.Vector((vertex1.co.x,vertex1.co.y,vertex1.co.z)),1,level)) and
    not(is_inside(mathutils.Vector((vertex2.co.x,vertex2.co.y,vertex2.co.z)),1,level))):
        f.write('<edge id="e'+str(idxe)+'" source="'+ str(edge.vertices[0])+'" target="'+str(edge.vertices[1])+'" />\n')
        
    

f.write('</graph>\n')
f.write('</graphml>\n')
f.close()

print('exportación finalizada')