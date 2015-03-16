// Win32TestConsole.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

int WinSock2VS2013Test(int argc, char* argv[]);

void TestUnorderedSet();


int _tmain(int argc, _TCHAR* argv[])
{
	/// int ret = WinSock2VS2013Test(argc, argv);
	TestUnorderedSet();
	::system("pause");
	return 0;
}

#include <winsock2.h>
#include <ws2tcpip.h>

// Minimum system required are
// Windows Server 2008 or Windows Vista
// http://msdn.microsoft.com/en-us/library/aa383745.aspx
// #define _WIN32_WINNT 0x0600

int WinSock2VS2013Test(int argc, char* argv[])
{
	WSADATA wsaData;

	struct addrinfo hints, *res, *p;
	int status;
	// Provide big enough buffer, ipv6 should be the biggest
	char ipstr[INET6_ADDRSTRLEN];
	char ipstr2[INET6_ADDRSTRLEN];

	if (argc != 2)
	{
		printf("Usage: %s <hostname>\n", argv[0]);
		printf("Example: %s www.yahoo.com\n", argv[0]);
		return 1;
	}

	// Initialization
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		printf("WSAStartup() failed miserably! With error code %ld\n", WSAGetLastError());
		return 1;
	}
	else
		printf("WSAStartup() looks fine!\n");

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
	{
		printf("getaddrinfo() failed lor! with error code %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	printf("The IP addresses for %s:\n", argv[1]);

	for (p = res; p != NULL; p = p->ai_next)
	{
		void *addr;
		char *ipver;

		// Get the pointer to the address itself, different fields in IPv4 and IPv6
		if (p->ai_family == AF_INET)
		{
			// IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		}
		else
		{
			// IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}

		// Convert the IP to a string and print it
		printf("String format: ");
		// InetNtop(address_family, IP_address_in_network_byte_to_convert_to_a_string,
		//         buffer_to_store_the_IP_address_string, the_IP_string_length_in_character);
		const char* ip = inet_ntop(p->ai_family, addr, (PSTR)ipstr, sizeof(ipstr));
		printf(" %s: %s\n", ipver, ipstr);

		printf("Value format: ");
		// InetPton(address_family, string_to_be_converted, buffer_to_store_the_converted_string);
		inet_pton(p->ai_family, (PCSTR)ipstr, ipstr2);
		printf(" %s: %ul\n", ipver, ipstr2);
	}

	// Deallocate the resource...
	freeaddrinfo(res);
	// Cleanup the WSA...
	WSACleanup();

	return 0;
}

#include <unordered_set>
#include <iostream>

//------------------------------------------------------------------------------
/**
	* 2015-03-16 : sbyang
	- unordered_set test
	unordered_set 은 추가된 순서대로 리스트상에 존재한다.
	iterator로 참조하면 추가된 순서대로 접근이 가능하다.
	중복 되는 요소를 다시 추가하면 실패 처리 되며 기존에 추가된 요소를 반환한다.
*/
void TestUnorderedSet()
{
	std::unordered_set<int> settest;
	settest.insert(30);
	settest.insert(40);
	auto ret1 = settest.insert(20);
	auto ret2 = settest.insert(40);

	for each (auto i in settest)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
}