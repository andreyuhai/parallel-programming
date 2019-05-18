### All 2 All Personalized Ring

In a ring network, each processor is creating an array of messages (sendArr[]) for all the processors (including itself).

The first element of the array is always the message meant for the processor itself which initialized the array or for the processor which has received that array.

After assigning the message at the appropriate index in the receive array (recvArr[]) the processor passes this message list to the next processor in the ring but the message sent will be a pointer to the second element in the array which will be the message meant for the next processor.

--- 

You can see below a diagram of what's explained above.

This diagram shows all the steps for this particular ring network.


![all to all personalized ring](https://github.com/andreyuhai/parallel-programming/blob/master/all-to-all-personalized-ring/diagram.png)
