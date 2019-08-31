bl_info = {
	"name": "skin Exporter",
	"version": (1, 0, 0),
	"blender": (2, 80, 0),
	"location": "File > Import-Export",
	"description": "Exports a skin for devilless",
	"category": "Import-Export",
}

import bpy
import bpy_extras.io_utils
from bpy_extras.io_utils import axis_conversion
import bmesh
import struct
import mathutils
from math import radians

def exportskin(context, filename):
	def rvec3d(v):
		return round(v[0], 6), round(v[1], 6), round(v[2], 6)

	obj = context.active_object

	depsgraph = context.evaluated_depsgraph_get()
	mesh_owner_object = obj.evaluated_get(depsgraph)
	mesh = mesh_owner_object.to_mesh()

	if not mesh:
		raise Exception("Error, could not get mesh data from active object")

	mesh.transform(mesh_owner_object.matrix_world)

	m = axis_conversion(from_forward = 'Y', from_up = 'Z', to_forward = 'Z', to_up = 'Y').to_4x4()
	mesh.transform(m)

	bm = bmesh.new()
	bm.from_mesh(mesh)
	bmesh.ops.triangulate(bm, faces=bm.faces[:])
	bm.to_mesh(mesh)
	bm.free()
	
	arma = mesh_owner_object.find_armature().data

	if not arma:
		raise Exception("Error, could not get aramture from active object")
	
	bdict = {}
	i = 0
	
	for bone in arma.bones:
		if bone.use_deform:
			bdict[bone.name] = i
			i += 1
	
	verts = []
	vdict = [{} for i in range(len(mesh.vertices))]
	tris = [[] for f in range(len(mesh.polygons))]
	vertCount = 0

	for poly in mesh.polygons:
		smooth = poly.use_smooth
		normal = poly.normal[:]
		normalKey = rvec3d(normal)

		for loop_index in range(poly.loop_start, poly.loop_start + poly.loop_total):
			vidx = mesh.loops[loop_index].vertex_index
			v = mesh.vertices[vidx]
			if smooth:
				normal = v.normal[:]
				normalKey = rvec3d(normal)
			
			groupId = [0, 0, 0]
			groupWeight = [0.0, 0.0, 0.0]
			
			if len(v.groups) > 3:
				raise Exception("Error, one or more vertices have more than 3 vertex groups")
			
			for i, groupElement in enumerate(v.groups):
				groupId[i] = bdict[obj.vertex_groups[groupElement.group].name]
				groupWeight[i] = groupElement.weight

			key = normalKey

			vdictLocal = vdict[vidx]
			dictVidx = vdictLocal.get(key)

			if dictVidx is None:
				dictVidx = vdictLocal[key] = vertCount
				verts.append((vidx, normal, groupId, groupWeight))
				vertCount += 1

			tris[poly.index].append(dictVidx)

	with open(filename, 'wb') as out:
		out.write(struct.pack('I', vertCount))
		out.write(struct.pack('I', len(tris) * 3))

		for i, v in enumerate(verts):
			# vertex
			out.write(struct.pack('f', mesh.vertices[v[0]].co.x))
			out.write(struct.pack('f', mesh.vertices[v[0]].co.y))
			out.write(struct.pack('f', mesh.vertices[v[0]].co.z))

			# normal
			out.write(struct.pack('f', v[1][0]))
			out.write(struct.pack('f', v[1][1]))
			out.write(struct.pack('f', v[1][2]))
			
			# group ids
			out.write(struct.pack('f', v[2][0]))
			out.write(struct.pack('f', v[2][1]))
			out.write(struct.pack('f', v[2][2]))
			
			# group weights
			out.write(struct.pack('f', v[3][0]))
			out.write(struct.pack('f', v[3][1]))
			out.write(struct.pack('f', v[3][2]))

		for tri in tris:
			out.write(struct.pack('H', tri[0]))
			out.write(struct.pack('H', tri[1]))
			out.write(struct.pack('H', tri[2]))
			

class write_skin(bpy.types.Operator, bpy_extras.io_utils.ExportHelper):
	'''skin exporter'''
	bl_idname = "export.skin"
	bl_label = 'Export skin'
	filename_ext = ".skin"

	def execute(self, context):
		exportskin(context, self.properties.filepath)
		return {'FINISHED'}

	def check(self, context):
		filepath = bpy.path.ensure_ext(self.filepath, '.skin')
		if filepath != self.filepath:
			self.filepath = filepath
			return True
		return False


def menu_func(self, context):
	self.layout.operator(write_skin.bl_idname, text="skin (.skin)")


def register():
	bpy.utils.register_class(write_skin)
	bpy.types.TOPBAR_MT_file_export.append(menu_func)


def unregister():
	bpy.utils.unregister_class(write_skin)
	bpy.types.TOPBAR_MT_file_export.remove(menu_func)


if __name__ == "__main__":
	register()
