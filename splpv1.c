/* 
 * SPLPv1.c
 * The file is part of practical task for System programming course. 
 * This file contains validation of SPLPv1 protocol. 
 */

/*
  Fiodar Pazhyvilka
  Group ¹ 13
*/

/*
---------------------------------------------------------------------------------------------------------------------------
# |      STATE      |         DESCRIPTION       |           ALLOWED MESSAGES            | NEW STATE | EXAMPLE
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
1 | INIT            | initial state             | A->B     CONNECT                      |     2     |
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
2 | CONNECTING      | client is waiting for con-| A<-B     CONNECT_OK                   |     3     |
  |                 | nection approval from srv |                                       |           |                      
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
3 | CONNECTED       | Connection is established | A->B     GET_VER                      |     4     |                     
  |                 |                           |        -------------------------------+-----------+----------------------
  |                 |                           |          One of the following:        |     5     |                      
  |                 |                           |          - GET_DATA                   |           |                      
  |                 |                           |          - GET_FILE                   |           |                      
  |                 |                           |          - GET_COMMAND                |           |
  |                 |                           |        -------------------------------+-----------+----------------------
  |                 |                           |          GET_B64                      |     6     |                      
  |                 |                           |        ------------------------------------------------------------------
  |                 |                           |          DISCONNECT                   |     7     |                                 
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
4 | WAITING_VER     | Client is waiting for     | A<-B     VERSION ver                  |     3     | VERSION 2                     
  |                 | server to provide version |          Where ver is an integer (>0) |           |                      
  |                 | information               |          value. Only a single space   |           |                      
  |                 |                           |          is allowed in the message    |           |                      
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
5 | WAITING_DATA    | Client is waiting for a   | A<-B     CMD data CMD                 |     3     | GET_DATA a GET_DATA 
  |                 | response from server      |                                       |           |                      
  |                 |                           |          CMD - command sent by the    |           |                      
  |                 |                           |           client in previous message  |           |                      
  |                 |                           |          data - string which contains |           |                      
  |                 |                           |           the following allowed cha-  |           |                      
  |                 |                           |           racters: small latin letter,|           |                     
  |                 |                           |           digits and '.'              |           |                      
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
6 | WAITING_B64_DATA| Client is waiting for a   | A<-B     B64: data                    |     3     | B64: SGVsbG8=                    
  |                 | response from server.     |          where data is a base64 string|           |                      
  |                 |                           |          only 1 space is allowed      |           |                      
--+-----------------+---------------------------+---------------------------------------+-----------+----------------------
7 | DISCONNECTING   | Client is waiting for     | A<-B     DISCONNECT_OK                |     1     |                      
  |                 | server to close the       |                                       |           |                      
  |                 | connection                |                                       |           |                      
---------------------------------------------------------------------------------------------------------------------------

IN CASE OF INVALID MESSAGE THE STATE SHOULD BE RESET TO 1 (INIT)

*/

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "splpv1.h"

#define _CRT_SECURE_NO_WARNINGS

int status = 1;

enum test_status invalid_message()
{
    status = 1;
    return MESSAGE_INVALID;
}

enum test_status valid_message(int status_number)
{
    status = status_number;
    return MESSAGE_VALID;
}

bool check_ver(const char* string)
{
    if (string == NULL)
        return false;

    for (int i = 8; string[i] != '\0'; ++i)
        if (string[i] < '0' || string[i] > '9')
            return true;

    return false;
}

enum test_status check_CONNECTED(struct Message* msg)
{
    if (msg->direction == B_TO_A)
        return invalid_message();

    if (strcmp(msg->text_message, "GET_VER") == 0)
        return valid_message(4);

    if (strcmp(msg->text_message, "GET_DATA") == 0 ||
        strcmp(msg->text_message, "GET_FILE") == 0 ||
        strcmp(msg->text_message, "GET_COMMAND") == 0)
        return valid_message(5);

    if (strcmp(msg->text_message, "GET_B64") == 0)
        return valid_message(6);

    if (strcmp(msg->text_message, "DISCONNECT") == 0)
        return valid_message(7);

    return invalid_message();
}

enum test_status check_WAITING_DATA(struct Message* msg)
{
    if (msg->direction == A_TO_B)
        return invalid_message();

    char buf[13] = "\0\0\0\0\0\0\0\0\0\0\0\0";
    int len_buf;
    if (strncmp(msg->text_message, "GET_DATA ", 9) == 0 )
    {
        len_buf = 9;
        strcpy(buf, " GET_DATA");
    }

    else
    {
        if (strncmp(msg->text_message, "GET_FILE ", 9) == 0)
        {
            len_buf = 9;
            strcpy(buf, " GET_FILE");
        }

        else
        {
            if (strncmp(msg->text_message, "GET_COMMAND ", 12) == 0)
            {
                len_buf = 12;
                strcpy(buf, " GET_COMMAND");
            }

            else
                return invalid_message();
        }

    }

    const bool mask[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1,
                         1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0 };

    char* string = msg->text_message + len_buf;
    while(*(string + len_buf) != '\0')
    {
        if (mask[*string] == false)
            return invalid_message();
        ++string;
    }

    if (strcmp(string, buf) != 0)
        return invalid_message();

    return valid_message(3);
}

enum test_status check_WAITING_B64_DATA(struct Message* msg) {
    if (msg->direction == A_TO_B || strncmp(msg->text_message, "B64: ", 5) != 0)
        return invalid_message();

    const bool mask[256] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                            0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
                            0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0};

    char *string = msg->text_message + 5;

    while (*(string + 2) != '\0') {
        if (mask[*string] == false)
            return invalid_message();
        ++string;
    }

    while (*string != '\0') {
        if (*string != '=' && mask[*string] == false)
            return invalid_message();
        ++string;
    }

    if (((string - msg->text_message - 1) & 3) != 0)
        return invalid_message();

    return valid_message(3);
}

enum test_status validate_message( struct Message *msg )
{
    if (msg == NULL || msg->text_message == NULL)
        return invalid_message();

    if (status == 1)
    {
        if (msg->direction == B_TO_A || strcmp(msg->text_message, "CONNECT") != 0)
            return invalid_message();

        return valid_message(2);
    }

    if (status == 2)
    {
        if (msg->direction == A_TO_B || strcmp(msg->text_message, "CONNECT_OK") != 0)
            return invalid_message();

        return valid_message(3);
    }

    if (status == 3)
        return check_CONNECTED(msg);

    if (status == 4)
    {
        if (msg->direction == A_TO_B || strncmp(msg->text_message, "VERSION ", 8) != 0 ||
                                        check_ver(msg->text_message))
            return invalid_message();

        return valid_message(3);
    }

    if (status == 5)
        return check_WAITING_DATA(msg);

    if (status == 6)
        return check_WAITING_B64_DATA(msg);

    if (status == 7)
    {
        if (msg->direction == A_TO_B || strcmp(msg->text_message, "DISCONNECT_OK") != 0)
            return invalid_message();

        return valid_message(1);
    }
}
