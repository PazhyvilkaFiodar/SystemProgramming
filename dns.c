#define _CRT_SECURE_NO_WARNINGS
#include "list.h"
#include "dns.h"
#include "hashtable.h"
#include "siteadress.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct hashTable* lib;
DNSHandle InitDNS()
{
  lib = (struct hashTable*)malloc(sizeof(struct hashTable));
  for (int i = 0; i < 15000; i++)
    lib->hashes[i].first = NULL;
  if (lib->hashes == NULL)
    return INVALID_DNS_HANDLE;
  return (DNSHandle)lib;
}
IPADDRESS ConvertFourDigitToIPv4(int first, int second, int third, int fourth)
{
  return (first << 24) | (second << 16) | (third << 8) | fourth;
}
void LoadHostsFile(DNSHandle hDNS, const char* hostsFilePath)
{
  if (hDNS != INVALID_DNS_HANDLE)
  {
    FILE* hosts = fopen(hostsFilePath, "r");
    if (hosts == NULL)
    {
      fclose(hosts);
      return;
    }
    struct siteAdress* adress = (struct siteAdress*)malloc(sizeof(struct siteAdress));
    while (!feof(hosts))
    {
      int first, second, third, fourth;
      char host[300];
      fscanf(hosts, "%d.%d.%d.%d    %s\n", &first, &second, &third, &fourth, host);
      IPADDRESS IP = ConvertFourDigitToIPv4(first, second, third, fourth);
      adress->IPv4Adress = IP;
      strcpy(adress->internetAdress, host);
      AddToHashTable(lib, adress);
    }
    fclose(hosts);
  }
}

IPADDRESS DnsLookUp(DNSHandle hDNS, const char* hostName)
{
  if (hDNS != INVALID_DNS_HANDLE)
  {
    HashValue hash = ComputeHash(hostName);
    struct listItem* siteInfo = FindByHostName(FindByHash(lib, hash), hostName);
    if (siteInfo == NULL)
      return INVALID_IP_ADDRESS;
    return siteInfo->value->IPv4Adress;
  }
  return INVALID_IP_ADDRESS;
}

void ShutdownDNS(DNSHandle hDNS)
{
  if (hDNS != 0)
  {
    for (int i = 0; i < 15000; i++)
    {
      struct listItem* temp = (struct listItem*)malloc(sizeof(struct listItem));
      while (lib->hashes[i].first != NULL)
      {
        temp = lib->hashes[i].first;
        lib->hashes[i].first = lib->hashes[i].first->next;
        free(temp);
      }
    }
    free(lib);
  }
}