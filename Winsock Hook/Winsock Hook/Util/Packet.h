#pragma once

#define MAX_PACKETSIZE 1024

class Packet
{
public:
	Packet();
	Packet(int PacketSize);
	Packet(BYTE* Packet, int PacketSize);
	~Packet();

	void SetPacketHeader();
	void AddByte(BYTE value);
	void AddInt32LE(int32_t value);
	void AddInt16LE(int16_t value);
	void AddString(std::string value);
	void AddCharArray(char* value);
	void SetByte(BYTE value, int pos);
	void UpdatePacketSize();
	BYTE* GetRawPacket();
	int GetRawSize();

private:
	BYTE* m_Packet; //Packet body
	BYTE* o_Packet; //Outgoing packet
	int m_PacketSize;
	int m_CurrentPos;
};