bl_info = {
    "name": "anim Exporter",
    "version": (1, 0, 0),
    "blender": (2, 80, 0),
    "location": "File > Import-Export",
    "description": "Exports an animation for devilless",
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
		
		boneTransform = axisConversion
		poseBone = obj.pose.bones[bone.name]
		
		if bone.parent:
			boneTransform = poseBone.parent.matrix.inverted_safe()
		else:
			boneTransform = axisConversion
		boneTransform = boneTransform @ poseBone.matrix
		
		out.write(struct.pack('f', 0.0))
		
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

		WriteBones(out, bdict, axisConversion, obj, bone.children)

def exportAnim(context, filename):
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


class write_anim(bpy.types.Operator, bpy_extras.io_utils.ExportHelper):
    '''anim exporter'''
    bl_idname = "export.anim"
    bl_label = 'Export anim'
    filename_ext = ".anim"

    def execute(self, context):
        exportAnim(context, self.properties.filepath)
        return {'FINISHED'}

    def check(self, context):
        filepath = bpy.path.ensure_ext(self.filepath, '.anim')
        if filepath != self.filepath:
            self.filepath = filepath
            return True
        return False


def menu_func(self, context):
    self.layout.operator(write_anim.bl_idname, text="anim (.anim)")


def register():
	bpy.utils.register_class(write_anim)
	bpy.types.TOPBAR_MT_file_export.append(menu_func)


def unregister():
	bpy.utils.unregister_class(write_anim)
	bpy.types.TOPBAR_MT_file_export.remove(menu_func)


if __name__ == "__main__":
	register()