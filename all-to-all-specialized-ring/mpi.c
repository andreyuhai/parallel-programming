#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void all2AllPersonalizedRing(int *sendMsg, int *recvMsg, int taskId, int comm_size);

int main(int argc, char **argv)
{
 MPI_Init(&argc, &argv);
 int taskId, comm_size;
 MPI_Comm_rank(MPI_COMM_WORLD, &taskId);
 MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

 int *sendArr = calloc(comm_size, sizeof(int));
 int *recvArr = calloc(comm_size, sizeof(int));

 all2AllPersonalizedRing(sendArr, recvArr, taskId, comm_size);

 for(int i = 0; i < comm_size; i++)
   printf("[Process ID %d]\t[%d]\t=\t%d\n", taskId, i, recvArr[i]);

 MPI_Finalize();
 
 return 0; 
}

void all2AllPersonalizedRing(int *sendArr, int *recvArr, int taskId, int comm_size) {

 /* Initializes sendArr for every processor.
  * When initializing the array the first element is always the message 
  * meant for the processor itself which has initialized the array.
  * For example for
  * Processor ID 0, sendArr = {0, 1, 2, 3, 4, 5, ..., comm_size - 1}
  * Processor ID 1, sendArr = {1001, 1002, 1003, 1004, ..., 1000 + comm_size - 1, 1000}
  * .
  * .
  * Processor ID comm_size - 1 sendArr = {...}
  * Here for processor ID 0 the first element (sendArr[0]) is the message meant for itself.
  * So processor ID 0 will take this message set it in the recvArray at an appropriate index which is the ID of the processor.
  * recvArr[0] keeps the message from 0th processor which is the processor itself.
  * In the same manner recv[1] keeps the message from 1st processor and so on and so forth.
 */
 for(int i = 0; i < comm_size; i++) {
   sendArr[i] = taskId * 1000 + ((i + taskId) % comm_size);
 }

 // First getting the first element and setting it at the appropriate index
 // As explained above the first message will always be meant for the processor itself which initialized the array  or received it.
 // This is the message for processor itself so we assign that value to the appropriate index in recvArr.
 recvArr[taskId] = sendArr[0]; 

 int destination = (taskId + 1) % comm_size; // Destination to send the message to which will always be the same in a ring network. Next processor.
 int source = taskId - 1; // Source to receive the message from which will always be the same in a ring network. Previous processor.

 if(source < 0)
  source += comm_size;
 for(int i = 0; i < comm_size - 1; i++) {
   // Sending our message to the next processor in the ring but since the first message was meant for us
   // We send a pointer to the second element in the sendArr which will be meant for the next processor.
   MPI_Send(sendArr + 1, comm_size - i, MPI_INT, destination, 0, MPI_COMM_WORLD);
   MPI_Recv(sendArr, comm_size - i, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   int recvIndex = taskId - i - 1;
   if(recvIndex < 0)
     recvIndex += comm_size;
   recvArr[recvIndex] = sendArr[0];
 }
}
 
