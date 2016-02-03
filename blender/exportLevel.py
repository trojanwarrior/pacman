import bpy
import mathutils

grafo = bpy.data.objects["grafo"]
mesh = grafo.data



        
    

def exportToGraphMl():

    levelName = grafo.data.name
    idRoadsGroup = grafo.vertex_groups.find('roads')
    idPacStartGroup = grafo.vertex_groups.find('pacmanstart')
    idBigPillGroup = grafo.vertex_groups.find('bigpill')    
    idPhantomStartGroup = grafo.vertex_groups.find('phantomstart')    
    f = open('/home/flush/CEDV/pacman/pacman/blender/'+levelName+'.xml','w')
    f.write('<?xml version="1.0" encoding="UTF-8"?>\n')
    f.write('<graphml xmlns="http://wgraphml.graphdrawing.org/xmlns"\n')  
    f.write('xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"\n')
    f.write('xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns\n')
    f.write('http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">\n')
    f.write('<key id="keyIdNodo" for="node" attr.name="id" attr.type="int"/>\n')
    f.write('<key id="keyX" for="node" attr.name="x" attr.type="string"/>\n')
    f.write('<key id="keyY" for="node" attr.name="y" attr.type="string"/>\n')
    f.write('<key id="keyZ" for="node" attr.name="z" attr.type="string"/>\n')
    f.write('<key id="keyType" for="node" attr.name="type" attr.type="string"/>\n')
    f.write('<key id="keyIdArista" for="edge" attr.name="idarista" attr.type="string"/>\n')
    f.write('<key id="keySource" for="edge" attr.name="source" attr.type="int"/>\n')
    f.write('<key id="keyTarget" for="edge" attr.name="target" attr.type="int"/>\n')
    f.write('<key id="keyWeight" for="edge" attr.name="weight" attr.type="int">\n')

    f.write('<default>1</default>\n')
    f.write('</key>')
    f.write('<graph id="'+levelName+'" edgedefault="undirected" >\n')
    for idx,ver in enumerate(mesh.vertices):
        if isVertexInGroup(idx,idRoadsGroup):
            f.write('<node id="'+str(idx)+'" >\n')
            f.write('<data id="keyIdNodo">'+str(idx)+'</data>\n')
            f.write('<data id="keyX">'+str(ver.co.x)+'</data>\n')
            f.write('<data id="keyY">'+str(ver.co.z)+'</data>\n')
            f.write('<data id="keyZ">'+str(ver.co.y)+'</data>\n')
            type='regular'
            if isVertexInGroup(idx,idPacStartGroup) == True:
                type = 'pacmanstart'
            elif isVertexInGroup(idx,idBigPillGroup) == True:
                type  = 'bigpill'
            elif isVertexInGroup(idx,idPhantomStartGroup) == True:
                type = 'phantomstart'
            f.write('<data id="keyType">'+type+'</data>\n')
            f.write('</node>\n')
    for idxe, edge in enumerate(mesh.edges):
        
        
        if isVertexInGroup(edge.vertices[0],idRoadsGroup) == True and isVertexInGroup(edge.vertices[1],idRoadsGroup) == True:
            f.write('<edge id="e'+str(idxe)+'" source="'+ str(edge.vertices[0])+'" target="'+str(edge.vertices[1])+'" >\n')
            f.write('<data id="keyIdArista">'+str(idxe)+'</data>\n')
            f.write('<data id="keySource">'+str(edge.vertices[0])+'</data>\n')
            f.write('<data id="keyTarget">'+str(edge.vertices[1])+'</data>\n')
            f.write('</edge>')

    f.write('</graph>\n')
    f.write('</graphml>\n')
    f.close()

    print('exportación finalizada')


def deselectAll():
    bpy.ops.mesh.select_all(action = 'DESELECT')


def selectGroup(group):
    deselectAll();
    bpy.ops.object.mode_set(mode="OBJECT")    
    grafo = bpy.context.object
    mesh = grafo.data
    vgnum = grafo.vertex_groups.find(group)
    

    for v in mesh.vertices:
         for g in v.groups:
             if g.group == vgnum:
                 v.select = True
    bpy.ops.object.mode_set(mode="EDIT")
    
            
def isVertexInGroup(idVer,idGroup):
    grafo = bpy.context.object
    mesh = grafo.data
    for g in mesh.vertices[idVer].groups:
        if g.group == idGroup:
            return True
             
    return False

#
# empty Group
#
def cleanGroup(group):
    grafo = bpy.context.object
    mesh = grafo.data
    bpy.ops.object.mode_set(mode="OBJECT")        
    vgnum = grafo.vertex_groups.find(group)
    if(vgnum < 0):
            return
    group = grafo.vertex_groups[vgnum]
    for idxe, ver in enumerate(mesh.vertices):
        if isVertexInGroup(idxe,vgnum) == True:
            group.remove([idxe])
    bpy.ops.object.mode_set(mode="EDIT")
    
    
#Add or remove a vertes from a group
def opSelectedToGroup(group,op):

    grafo = bpy.context.object
    mesh = grafo.data
    bpy.ops.object.mode_set(mode="OBJECT")        
    vgnum = grafo.vertex_groups.find(group)
    if(vgnum < 0):
        if op == 'ADD':
            grafo.vertex_groups.new(group)
            vgnum = grafo.vertex_groups.find(group)
        else:
            return;
    bpy.ops.object.mode_set(mode="EDIT")

    group = grafo.vertex_groups[vgnum]

    vertexToOp = []
    bpy.ops.object.mode_set(mode="EDIT")
    for idxe, ver in enumerate(mesh.vertices):
        if ver.select == True:
            vertexToOp.append(idxe)            
    bpy.ops.object.mode_set(mode="OBJECT")
    
    for verIndex in vertexToOp:
        if op == 'ADD':
            if isVertexInGroup(verIndex,vgnum) == False:
                group.add([verIndex],0,'ADD')
            else:
                group.add([verIndex],0,'REPLACE')
        else:
               group.add([verIndex],0,'SUBTRACT')
    bpy.ops.object.mode_set(mode="EDIT")    
    

#
# Generate level meshes
#
def generateLevel():
    grafo = bpy.context.object
    mesh = grafo.data
    bpy.ops.mesh.select_all(action = 'DESELECT')
    bpy.ops.object.mode_set(mode="OBJECT")    
    vgnum = grafo.vertex_groups.find('walls')
    first = True
    cubes = []
    for v in mesh.vertices:
         for g in v.groups:
             if g.group == vgnum:
                 bpy.ops.mesh.primitive_cube_add(radius=0.26,location=(v.co.x,v.co.y,v.co.z))
                 cube = bpy.context.object
                 cubes.append(cube)
                 if first == True:
                     first = False
                     bpy.context.scene.objects.active = cube
    for cube in cubes:
        cube.select = True                     
    bpy.ops.object.join()
    bpy.context.object.data.materials.append(bpy.data.materials["MatLevel"])
                 

    
    
      
#
# mark vertices as Walls
#
class MarkAsWalls(bpy.types.Operator):
    """ This operator mark selected vertex as walls

    """
    bl_idname = "pacman.markwalls"
    bl_label = "Mark as Walls"
 
    def invoke(self, context, event):
        opSelectedToGroup("roads","REMOVE")        
        opSelectedToGroup("walls","ADD")
        return {'FINISHED'}

#
# Select Vertex from Wall
#
class SelectWalls(bpy.types.Operator):
    """ This operator select walls

    """
    bl_idname = "pacman.selectwalls"
    bl_label = "Select Walls"
 
 
    def invoke(self, context, event):
        selectGroup("walls")
        return {'FINISHED'}
 
 
    
#
# mark vertices as roads
#
class MarkAsRoads(bpy.types.Operator):
    """ This operator mark selected vertex as roads

    """
    bl_idname = "pacman.markroads"
    bl_label = "Mark as Roads"
 
    def invoke(self, context, event):
        opSelectedToGroup("walls","REMOVE")
        opSelectedToGroup("roads","ADD")     
        return {'FINISHED'}

#
# Select Vertex from Roads
#
class SelectRoads(bpy.types.Operator):
    """ This operator select roads

    """
    bl_idname = "pacman.selectroads"
    bl_label = "Select Roads"
 

    def invoke(self, context, event):
        selectGroup("roads")
        return {'FINISHED'}
 
#
# Generate level Mesh
#
class GenerateLevel(bpy.types.Operator):
    '''
        This operator generate pacman level
    '''
    bl_idname = "pacman.generatelevel"
    bl_label = "Generate Level"
    def invoke(self, context, event):
        generateLevel()
        return {'FINISHED'}



    


#
# mark vertex to selected type
#
class MarkGroup(bpy.types.Operator):
    '''
        This operator mark the vertex to a selected type
    '''
    bl_idname = "pacman.mark"
    bl_label = "Mark"

    def invoke(self, context, event):
        selGroup = context.scene.pacman_tiles_types
        opSelectedToGroup(selGroup,'ADD')
        return {'FINISHED'}


#
# mark vertex to selected type
#
class CleanGroup(bpy.types.Operator):
    '''
        This operator clenas all the vertex to a selected type
    '''
    bl_idname = "pacman.clean"
    bl_label = "Clean"

    def invoke(self, context, event):
        selGroup = context.scene.pacman_tiles_types
        cleanGroup(selGroup)
        return {'FINISHED'}



#
# Select Group
#
class SelectGroup(bpy.types.Operator):
    '''
        This operator select all the vertex from a selected type
    '''
    bl_idname = "pacman.select"
    bl_label = "Select"

    def invoke(self, context, event):
        selGroup = context.scene.pacman_tiles_types

        selectGroup(str(selGroup))
        return {'FINISHED'}


#
# Export to Graphml
#
class ExportGraph(bpy.types.Operator):
    '''
        This operator exports the graphml format     
    '''
    bl_idname = "pacman.export"
    bl_label = "Export Graph"

    def invoke(self, context, event):
        exportToGraphMl()        
        return {'FINISHED'}



#
#    Pacman Editors panel
#
class PacmanEditor(bpy.types.Panel):
    bl_label = "PacmanEditor"
    bl_space_type = "VIEW_3D"
    bl_region_type = "TOOL_PROPS"    
 
 
    def draw(self, context):
        scn = context.scene
    

        self.layout.operator("pacman.selectwalls")
        self.layout.operator("pacman.selectroads")
        self.layout.operator("pacman.markwalls")
        self.layout.operator("pacman.markroads")
        self.layout.prop(scn, 'pacman_tiles_types')



        self.layout.operator("pacman.select")
        self.layout.operator("pacman.mark")
        self.layout.operator("pacman.clean")
        self.layout.operator("pacman.generatelevel")
        self.layout.operator("pacman.export")       

 
# Registration
pacman_tiles_types = [("bigpill","bigpill","",1),("phantomstart","phantomstart","",2),("pacmanstart","pacmanstart","",3)]

bpy.types.Scene.pacman_tiles_types = bpy.props.EnumProperty(name = "Type", default = "bigpill", items = pacman_tiles_types)    


bpy.utils.register_class(GenerateLevel)
bpy.utils.register_class(SelectWalls)
bpy.utils.register_class(MarkAsWalls)
bpy.utils.register_class(MarkAsRoads)
bpy.utils.register_class(SelectRoads)
bpy.utils.register_class(SelectGroup)
bpy.utils.register_class(CleanGroup)
bpy.utils.register_class(MarkGroup)
bpy.utils.register_class(ExportGraph)


bpy.utils.register_class(PacmanEditor)