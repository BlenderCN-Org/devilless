bl_info = {
    "name": "skel Exporter",
    "version": (1, 0, 0),
    "blender": (2, 80, 0),
    "location": "File > Import-Export",
    "description": "Exports a skeleton for devilless",
    "category": "Import-Export",
}

import bpy
import bpy_extras.io_utils
from bpy_extras.io_utils import axis_conversion
import struct
from mathutils import *
from math import radians, pi

def WriteBones(out, bdict, axisConversion, obj, bones):
	for bone in bones:
		if not bone.use_deform:
			continue
		
		poseBone = obj.pose.bones[bone.name]
		boneTransform = poseBone.matrix.inverted_safe()
		boneTransform = boneTransform @ axisConversion
		
		out.write(struct.pack('f', boneTransform[0][0]))
		out.write(struct.pack('f', boneTransform[0][1]))
		out.write(struct.pack('f', boneTransform[0][2]))
		out.write(struct.pack('f', boneTransform[0][3]))
		
		out.write(struct.pack('f', boneTransform[1][0]))
		out.write(struct.pack('f', boneTransform[1][1]))
		out.write(struct.pack('f', boneTransform[1][2]))
		out.write(struct.pack('f', boneTransform[1][3]))
		
		out.write(struct.pack('f', boneTransform[2][0]))
		out.write(struct.pack('f', boneTransform[2][1]))
		out.write(struct.pack('f', boneTransform[2][2]))
		out.write(struct.pack('f', boneTransform[2][3]))
		
		out.write(struct.pack('f', boneTransform[3][0]))
		out.write(struct.pack('f', boneTransform[3][1]))
		out.write(struct.pack('f', boneTransform[3][2]))
		out.write(struct.pack('f', boneTransform[3][3]))
		
		out.write(struct.pack('I', len(bone.children)))
		
		for child in bone.children:
			out.write(struct.pack('I', bdict[child.name]))

		WriteBones(out, bdict, axisConversion, obj, bone.children)

def exportSkel(context, filename):
	obj = context.active_object
	arma = context.active_object.data
	
	if not arma:
		raise Exception("Error, could not find an armature from the active object")
		
	axisConversion = axis_conversion(from_forward = 'Y', from_up = 'Z', to_forward = 'Z', to_up = 'Y').to_4x4()

	with open(filename, 'wb') as out:
		bdict = {}
		i = 0
		
		for bone in arma.bones:
			if bone.use_deform:
				bdict[bone.name] = i
				i += 1

		out.write(struct.pack('I', i))
		
		rootBones = [Bone for Bone in arma.bones if Bone.parent is None]
		WriteBones(out, bdict, axisConversion, obj, rootBones)


class write_skel(bpy.types.Operator, bpy_extras.io_utils.ExportHelper):
    '''skel exporter'''
    bl_idname = "export.skel"
    bl_label = 'Export skel'
    filename_ext = ".skel"

    def execute(self, context):
        exportSkel(context, self.properties.filepath)
        return {'FINISHED'}

    def check(self, context):
        filepath = bpy.path.ensure_ext(self.filepath, '.skel')
        if filepath != self.filepath:
            self.filepath = filepath
            return True
        return False


def menu_func(self, context):
    self.layout.operator(write_skel.bl_idname, text="skel (.skel)")


def register():
	bpy.utils.register_class(write_skel)
	bpy.types.TOPBAR_MT_file_export.append(menu_func)


def unregister():
	bpy.utils.unregister_class(write_skel)
	bpy.types.TOPBAR_MT_file_export.remove(menu_func)


if __name__ == "__main__":
	register()