#ifndef STUN_TYPES_H
#define STUN_TYPES_H

//https://www.rfc-editor.org/rfc/rfc3489#section-11.1
enum class StunMessageType : short
{
	BindingRequest = 0x0001,
	BindingResponse = 0x0101,
	BindingErrorResponse = 0x0111,
	SharedSecretRequest = 0x0002,
	SharedSecretResponse = 0x0102,
	SharedSecretErrorResponse = 0x0112
};

//https://www.rfc-editor.org/rfc/rfc3489#section-11.2
enum class StunAttributeType : short
{
	MappedAddress = 0x0001,
	ResponseAddress = 0x0002,
	ChangeRequest = 0x0003,
	SourceAddress = 0x0004,
	ChangedAddress = 0x0005,
	Username = 0x0006,
	Password = 0x0007,
	MessageIntegrity = 0x0008,
	ErrorCode = 0x0009,
	UnknownAttributes = 0x000a,
	ReflectedFrom = 0x000b
};

#endif
