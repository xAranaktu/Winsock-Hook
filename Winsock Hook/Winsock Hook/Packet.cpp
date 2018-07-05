#include <Windows.h>
#include <stdint.h>
#include <string>
#include "Util\Hook.h"
#include "Util\Packet.h"

Packet::Packet()
{
	m_Packet = new BYTE[MAX_PACKETSIZE];
	o_Packet = 0;
	m_PacketSize = 0;
	m_CurrentPos = 0;
	SetPacketHeader();
}

Packet::Packet(int PacketSize) {
	m_Packet = new BYTE[PacketSize];
	o_Packet = 0;
	m_PacketSize = 0;
	m_CurrentPos = 0;
	SetPacketHeader();
}

Packet::Packet(BYTE* Packet, int PacketSize) {
	m_Packet = Packet;
	m_PacketSize = PacketSize;
	m_CurrentPos = 0;
	SetPacketHeader();
}

void Packet::SetPacketHeader() {
	AddInt32LE(m_PacketSize);
	AddInt32LE(0);	//Unknown 4 bytes
	AddInt32LE(0);	// Unknown 4 bytes
}

void Packet::AddByte(BYTE value) {
	m_Packet[m_CurrentPos] = value;
	m_CurrentPos++;
	m_PacketSize++;
}

void Packet::AddInt32LE(int32_t value) {
	m_Packet[m_CurrentPos] = (value >> 24) & 0xFF;
	m_Packet[m_CurrentPos+1] = (value >> 16) & 0xFF;
	m_Packet[m_CurrentPos+2] = (value >> 8) & 0xFF;
	m_Packet[m_CurrentPos+3] = value & 0xFF;

	m_CurrentPos += 4;
	m_PacketSize += 4;
}

void Packet::AddInt16LE(int16_t value) {
	m_Packet[m_CurrentPos + 1] = (value >> 8) & 0xFF;
	m_Packet[m_CurrentPos] = value & 0xFF;

	m_CurrentPos += 2;
	m_PacketSize += 2;
}

void Packet::AddString(std::string value) {
	for (std::string::iterator it = value.begin(); it != value.end(); ++it) {
		AddByte(*it);
	}
}

void Packet::AddCharArray(char *value) {
	for (char* it = value; *it; ++it) {
		AddByte(*it);
	}
}

void Packet::SetByte(BYTE value, int pos) {
	m_Packet[pos] = value;
}

void Packet::UpdatePacketSize() {
	SetByte((m_PacketSize >> 24) & 0xFF, 3);
	SetByte((m_PacketSize >> 16) & 0xFF, 2);
	SetByte((m_PacketSize >> 8) & 0xFF, 1);
	SetByte(m_PacketSize & 0xFF, 0);
}

BYTE* Packet::GetRawPacket() {
	o_Packet = new BYTE[m_PacketSize];
	memcpy(o_Packet, m_Packet, m_PacketSize);
	return o_Packet;
}

int Packet::GetRawSize() {
	return m_PacketSize;
}

Packet::~Packet()
{
	delete[] m_Packet;
	if (o_Packet != 0)
		delete[] o_Packet;
}