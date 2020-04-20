#include "PbJsonConvert.h"

void PbJsonConvert::PbMsg2JsonStr(const ProtobufMsg& src, std::string& dst, bool enum2str)
{
	Json::Value value;
	PbMsg2Json(src, value, enum2str);
	Json::FastWriter writer;
	dst = writer.write(value);
}

bool PbJsonConvert::JsonStr2PbMsg(const std::string& src, ProtobufMsg& dst, bool str2enum)
{
	Json::Value value;
	Json::Reader reader(Json::Features::strictMode());
	if (!reader.parse(src, value))
	{
		return false;
	}
	if (true != Json2PbMsg(value, dst, str2enum))
	{
		return false;
	}
	return true;
}

bool PbJsonConvert::Json2PbMsg(const Json::Value& src, ProtobufMsg& dst, bool str2enum)
{
	const ProtobufDescriptor* descriptor = dst.GetDescriptor();
	const ProtobufReflection* reflection = dst.GetReflection();
	if (NULL == descriptor || NULL == reflection) return false;

	int32_t count = descriptor->field_count();
	for (int32_t i = 0; i < count; ++i)
	{
		const ProtobufFieldDescriptor* field = descriptor->field(i);
		if (NULL == field) continue;

		if (!src.isMember(field->name()))
		{
			continue;
		}
		const Json::Value& value = src[field->name()];

		if (field->is_repeated())
		{
			if (!value.isArray())
			{
				return false;
			}
			else
			{
				Json2RepeatedMessage(value, dst, field, reflection, str2enum);
				continue;
			}
		}
		switch (field->type())
		{
		case ProtobufFieldDescriptor::TYPE_BOOL:
		{
			if (value.isBool())
			{
				reflection->SetBool(&dst, field, value.asBool());
			}

			else if (value.isInt())
			{
				reflection->SetBool(&dst, field, value.isInt());
			}

			else if (value.isString())
			{
				if (value.asString() == "true")
					reflection->SetBool(&dst, field, true);
				else if (value.asString() == "false")
					reflection->SetBool(&dst, field, false);
			}
			break;
		}

		case ProtobufFieldDescriptor::TYPE_INT32:
		case ProtobufFieldDescriptor::TYPE_SINT32:
		case ProtobufFieldDescriptor::TYPE_SFIXED32:
		{
			if (value.isInt()) reflection->SetInt32(&dst, field, value.asInt());
			break;
		}

		case ProtobufFieldDescriptor::TYPE_UINT32:
		case ProtobufFieldDescriptor::TYPE_FIXED32:
		{
			if (value.isUInt()) reflection->SetUInt32(&dst, field, value.asUInt());
			break;
		}

		case ProtobufFieldDescriptor::TYPE_INT64:
		case ProtobufFieldDescriptor::TYPE_SINT64:
		case ProtobufFieldDescriptor::TYPE_SFIXED64:
		{
			if (value.isInt()) reflection->SetInt64(&dst, field, value.asInt64());
			break;
		}
		case ProtobufFieldDescriptor::TYPE_UINT64:
		case ProtobufFieldDescriptor::TYPE_FIXED64:
		{
			if (value.isUInt()) reflection->SetUInt64(&dst, field, value.asUInt64());
			break;
		}

		case ProtobufFieldDescriptor::TYPE_FLOAT:
		{
			if (value.isDouble()) reflection->SetFloat(&dst, field, value.asFloat());
			break;
		}

		case ProtobufFieldDescriptor::TYPE_DOUBLE:
		{
			if (value.isDouble()) reflection->SetDouble(&dst, field, value.asDouble());
			break;
		}

		case ProtobufFieldDescriptor::TYPE_STRING:
		case ProtobufFieldDescriptor::TYPE_BYTES:
		{
			if (value.isString()) reflection->SetString(&dst, field, value.asString());
			break;
		}

		case ProtobufFieldDescriptor::TYPE_MESSAGE:
		{
			if (value.isObject()) Json2PbMsg(value, *reflection->MutableMessage(&dst, field));
			break;
		}
		default:
		{
			break;
		}
		}
	}
	return true;
}

void PbJsonConvert::PbMsg2Json(const ProtobufMsg& src, Json::Value& dst, bool enum2str)
{
	const ProtobufDescriptor* descriptor = src.GetDescriptor();
	const ProtobufReflection* reflection = src.GetReflection();
	if (descriptor == nullptr || reflection == nullptr) return;

	int32_t count = descriptor->field_count();

	for (int32_t i = 0; i < count; ++i)
	{
		const ProtobufFieldDescriptor* field = descriptor->field(i);

		if (field->is_repeated())
		{
			if (reflection->FieldSize(src, field) > 0)
				RepeatedMessage2Json(src, field, reflection, dst[field->name()], enum2str);
			continue;
		}


		if (!reflection->HasField(src, field))
		{
			continue;
		}

		switch (field->type())
		{
		case ProtobufFieldDescriptor::TYPE_MESSAGE:
		{
			const ProtobufMsg& tmp_message = reflection->GetMessage(src, field);
			if (0 != tmp_message.ByteSizeLong()) PbMsg2Json(tmp_message, dst[field->name()]);
			break;
		}

		case ProtobufFieldDescriptor::TYPE_BOOL:
			dst[field->name()] = reflection->GetBool(src, field) ? true : false;
			break;

		case ProtobufFieldDescriptor::TYPE_ENUM:
		{
			const ::google::protobuf::EnumValueDescriptor* enum_value_desc = reflection->GetEnum(src, field);
			if (enum2str) {
				dst[field->name()] = enum_value_desc->name();
			}
			else {
				dst[field->name()] = enum_value_desc->number();
			}
			break;
		}

		case ProtobufFieldDescriptor::TYPE_INT32:
		case ProtobufFieldDescriptor::TYPE_SINT32:
		case ProtobufFieldDescriptor::TYPE_SFIXED32:
			dst[field->name()] = Json::Int(reflection->GetInt32(src, field));
			break;

		case ProtobufFieldDescriptor::TYPE_UINT32:
		case ProtobufFieldDescriptor::TYPE_FIXED32:
			dst[field->name()] = Json::UInt(reflection->GetUInt32(src, field));
			break;

		case ProtobufFieldDescriptor::TYPE_INT64:
		case ProtobufFieldDescriptor::TYPE_SINT64:
		case ProtobufFieldDescriptor::TYPE_SFIXED64:
			dst[field->name()] = Json::Int64(reflection->GetInt64(src, field));
			break;

		case ProtobufFieldDescriptor::TYPE_UINT64:
		case ProtobufFieldDescriptor::TYPE_FIXED64:
			dst[field->name()] = Json::UInt64(reflection->GetUInt64(src, field));
			break;

		case ProtobufFieldDescriptor::TYPE_FLOAT:
			dst[field->name()] = reflection->GetFloat(src, field);
			break;

		case ProtobufFieldDescriptor::TYPE_STRING:
		case ProtobufFieldDescriptor::TYPE_BYTES:
			dst[field->name()] = reflection->GetString(src, field);
			break;

		default:
			break;
		}
	}
}

bool PbJsonConvert::Json2RepeatedMessage(const Json::Value& json, ProtobufMsg& message,
	const ProtobufFieldDescriptor* field,
	const ProtobufReflection* reflection,
	bool str2enum)
{
	int32_t count = json.size();
	for (int32_t j = 0; j < count; ++j)
	{
		switch (field->type())
		{
		case ProtobufFieldDescriptor::TYPE_BOOL:
		{
			if (json.isBool())
			{
				reflection->AddBool(&message, field, json[j].asBool());
			}
			else if (json[j].isInt())
			{
				reflection->AddBool(&message, field, json[j].asInt());
			}
			else if (json[j].isString())
			{
				if (json[j].asString() == "true")
				{
					reflection->AddBool(&message, field, true);
				}
				else if (json[j].asString() == "false")
				{
					reflection->AddBool(&message, field, false);
				}
			}
			break;
		}

		case ProtobufFieldDescriptor::TYPE_ENUM:
		{
			const ::google::protobuf::EnumDescriptor* pedesc = field->enum_type();
			const ::google::protobuf::EnumValueDescriptor* pevdesc = NULL;
			if (str2enum)
			{
				pevdesc = pedesc->FindValueByName(json[j].asString());

			}
			else
			{
				pevdesc = pedesc->FindValueByNumber(json[j].asInt());
			}
			if (NULL != pevdesc)
			{
				reflection->AddEnum(&message, field, pevdesc);
			}
			break;
		}

		case ProtobufFieldDescriptor::TYPE_INT32:
		case ProtobufFieldDescriptor::TYPE_SINT32:
		case ProtobufFieldDescriptor::TYPE_SFIXED32:
		{
			if (json[j].isInt()) reflection->AddInt32(&message, field, json[j].asInt());
		} break;

		case ProtobufFieldDescriptor::TYPE_UINT32:
		case ProtobufFieldDescriptor::TYPE_FIXED32:
		{
			if (json[j].isUInt()) reflection->AddUInt32(&message, field, json[j].asUInt());
		} break;

		case ProtobufFieldDescriptor::TYPE_INT64:
		case ProtobufFieldDescriptor::TYPE_SINT64:
		case ProtobufFieldDescriptor::TYPE_SFIXED64:
		{
			if (json[j].isInt()) reflection->AddInt64(&message, field, json[j].asInt64());
		} break;

		case ProtobufFieldDescriptor::TYPE_UINT64:
		case ProtobufFieldDescriptor::TYPE_FIXED64:
		{
			if (json[j].isUInt()) reflection->AddUInt64(&message, field, json[j].asUInt64());
		} break;

		case ProtobufFieldDescriptor::TYPE_FLOAT:
		{
			if (json[j].isDouble()) reflection->AddFloat(&message, field, json[j].asFloat());
		} break;

		case ProtobufFieldDescriptor::TYPE_DOUBLE:
		{
			if (json[j].isDouble()) reflection->AddDouble(&message, field, json[j].asDouble());
		} break;

		case ProtobufFieldDescriptor::TYPE_MESSAGE:
		{
			if (json[j].isObject()) Json2PbMsg(json[j], *reflection->AddMessage(&message, field));
		} break;

		case ProtobufFieldDescriptor::TYPE_STRING:
		case ProtobufFieldDescriptor::TYPE_BYTES:
		{
			if (json[j].isString()) reflection->AddString(&message, field, json[j].asString());
		} break;

		default:
		{
			break;
		}
		}
	}
	return true;
}

void PbJsonConvert::RepeatedMessage2Json(const ProtobufMsg& message,
	const ProtobufFieldDescriptor* field,
	const ProtobufReflection* reflection,
	Json::Value& json, bool enum2str)
{
	if (NULL == field || NULL == reflection)
	{
		PbMsg2Json(message, json);
	}

	for (int32_t i = 0; i < reflection->FieldSize(message, field); ++i)
	{
		Json::Value tmp_json;
		switch (field->type())
		{
		case ProtobufFieldDescriptor::TYPE_MESSAGE:
		{
			const ProtobufMsg& tmp_message = reflection->GetRepeatedMessage(message, field, i);
			if (0 != tmp_message.ByteSizeLong())
			{
				PbMsg2Json(tmp_message, tmp_json);
			}
			break;
		}

		case ProtobufFieldDescriptor::TYPE_BOOL:
			tmp_json[field->name()] = reflection->GetRepeatedBool(message, field, i) ? true : false;
			break;

		case ProtobufFieldDescriptor::TYPE_ENUM:
		{
			const ::google::protobuf::EnumValueDescriptor* enum_value_desc = reflection->GetRepeatedEnum(message, field, i);
			if (enum2str)
			{
				tmp_json = enum_value_desc->name();
			}
			else
			{
				tmp_json = enum_value_desc->number();
			}
			break;
		}

		case ProtobufFieldDescriptor::TYPE_INT32:
		case ProtobufFieldDescriptor::TYPE_SINT32:
		case ProtobufFieldDescriptor::TYPE_SFIXED32:
			tmp_json[field->name()] = reflection->GetRepeatedInt32(message, field, i);
			break;

		case ProtobufFieldDescriptor::TYPE_UINT32:
		case ProtobufFieldDescriptor::TYPE_FIXED32:
			tmp_json[field->name()] = reflection->GetRepeatedUInt32(message, field, i);
			break;

		case ProtobufFieldDescriptor::TYPE_INT64:
		case ProtobufFieldDescriptor::TYPE_SINT64:
		case ProtobufFieldDescriptor::TYPE_SFIXED64:
			tmp_json[field->name()] = (Json::Int64)reflection->GetRepeatedInt64(message, field, i);
			break;

		case ProtobufFieldDescriptor::TYPE_UINT64:
		case ProtobufFieldDescriptor::TYPE_FIXED64:
			tmp_json[field->name()] = Json::UInt64(reflection->GetRepeatedUInt64(message, field, i));
			break;

		case ProtobufFieldDescriptor::TYPE_FLOAT:
			tmp_json[field->name()] = reflection->GetRepeatedFloat(message, field, i);
			break;

		case ProtobufFieldDescriptor::TYPE_STRING:
		case ProtobufFieldDescriptor::TYPE_BYTES:
			tmp_json[field->name()] = reflection->GetRepeatedString(message, field, i);
			break;

		default:
			break;
		}
		json.append(tmp_json);
	}
}