#ifndef COMM_UTILS_H
#define COMM_UTILS_H

#define NO_MESSAGE 0
#define MESSAGE_ACK 1

#define MESSAGE_GAME_STARTED 11
#define MESSAGE_SCORE 12
#define MESSAGE_DEATH 13

#define MESSAGE_GAME_START 21
#define MESSAGE_MAX_SCORE 22
#define MESSAGE_GAME_END 23

/**
 * Will generate message coresponding to that code
 * \param code code for the message to be generated
 * \return NULL if memory could not be allocated, otherwise pointer to message string that needs to be manually freed
 */
char * generate_message(int code);

/**
 * Will try to get message from specific buffer
 * \param buf buffer for the tcp communication
 * \param param uninitialized array of chars that will be initialized when there is an additional param for the message that needs to be freed after
 * \return -1 on error and 0 if there is no message and otherwise message code
 */
int check_for_message(char *buf, char *param);

/**
 * If message was read an processed then this function will clear the buffer
 * \return Amount of characters deleted from the buffer that need to be substracted from buff_i
 */
int clear_message(char *buf); 

#endif
