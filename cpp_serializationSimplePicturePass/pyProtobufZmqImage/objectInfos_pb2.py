# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: objectInfos.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='objectInfos.proto',
  package='proto',
  syntax='proto2',
  serialized_options=None,
  serialized_pb=_b('\n\x11objectInfos.proto\x12\x05proto\"\x9e\x01\n\x0bobjectInfos\x12,\n\x05infos\x18\x01 \x03(\x0b\x32\x1d.proto.objectInfos.objectInfo\x1a\x61\n\nobjectInfo\x12\x10\n\x08objClass\x18\x01 \x01(\x05\x12\r\n\x05score\x18\x02 \x01(\x02\x12\x0b\n\x03tlx\x18\x03 \x01(\x05\x12\x0b\n\x03tly\x18\x04 \x01(\x05\x12\x0b\n\x03\x62rx\x18\x05 \x01(\x05\x12\x0b\n\x03\x62ry\x18\x06 \x01(\x05')
)




_OBJECTINFOS_OBJECTINFO = _descriptor.Descriptor(
  name='objectInfo',
  full_name='proto.objectInfos.objectInfo',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='objClass', full_name='proto.objectInfos.objectInfo.objClass', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='score', full_name='proto.objectInfos.objectInfo.score', index=1,
      number=2, type=2, cpp_type=6, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='tlx', full_name='proto.objectInfos.objectInfo.tlx', index=2,
      number=3, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='tly', full_name='proto.objectInfos.objectInfo.tly', index=3,
      number=4, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='brx', full_name='proto.objectInfos.objectInfo.brx', index=4,
      number=5, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='bry', full_name='proto.objectInfos.objectInfo.bry', index=5,
      number=6, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=90,
  serialized_end=187,
)

_OBJECTINFOS = _descriptor.Descriptor(
  name='objectInfos',
  full_name='proto.objectInfos',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='infos', full_name='proto.objectInfos.infos', index=0,
      number=1, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[_OBJECTINFOS_OBJECTINFO, ],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=29,
  serialized_end=187,
)

_OBJECTINFOS_OBJECTINFO.containing_type = _OBJECTINFOS
_OBJECTINFOS.fields_by_name['infos'].message_type = _OBJECTINFOS_OBJECTINFO
DESCRIPTOR.message_types_by_name['objectInfos'] = _OBJECTINFOS
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

objectInfos = _reflection.GeneratedProtocolMessageType('objectInfos', (_message.Message,), dict(

  objectInfo = _reflection.GeneratedProtocolMessageType('objectInfo', (_message.Message,), dict(
    DESCRIPTOR = _OBJECTINFOS_OBJECTINFO,
    __module__ = 'objectInfos_pb2'
    # @@protoc_insertion_point(class_scope:proto.objectInfos.objectInfo)
    ))
  ,
  DESCRIPTOR = _OBJECTINFOS,
  __module__ = 'objectInfos_pb2'
  # @@protoc_insertion_point(class_scope:proto.objectInfos)
  ))
_sym_db.RegisterMessage(objectInfos)
_sym_db.RegisterMessage(objectInfos.objectInfo)


# @@protoc_insertion_point(module_scope)
