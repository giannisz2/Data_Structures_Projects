# Κάνοντας compile το <foo>_test.c με μια υλοποίηση <foo>.c του
# συγκεκριμένου τύπου, παράγουμε ένα tets για την υλοποίηση αυτή.

# Υλοποιήσεις μέσω Heap: ADTPriorityQueue
# Το Heap αλλά και το test χρησιμοποιούν Vector, οπότε χρειαζόμαστε και μια υλοποίηση του ADTVector.
#
UsingHeap_ADTPriorityQueue_test_OBJS    = ADTPriorityQueue_test.o $(MODULES)/UsingHeap/ADTPriorityQueue.o $(MODULES)/UsingDynamicArray/ADTVector.o

# Υλοποιήσεις μέσω ADTList: ADTPriorityQueue
# Η υλοποίηση χρησιμοποιεί List, οπότε χρειαζόμαστε και μια υλοποίηση του ADTList.
# Το test χρησιμοποιεί Vector, οπότε χρειαζόμαστε και μια υλοποίηση του ADTVector.
#
UsingADTList_ADTPriorityQueue_test_OBJS = ADTPriorityQueue_test.o $(MODULES)/UsingADTList/ADTPriorityQueue.o $(MODULES)/UsingLinkedList/ADTList.o $(MODULES)/UsingDynamicArray/ADTVector.o

# Ο βασικός κορμός του Makefile
include ../common.mk