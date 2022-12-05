# Κάνοντας compile το <foo>_test.c με μια υλοποίηση <foo>.c του
# συγκεκριμένου τύπου, παράγουμε ένα tets για την υλοποίηση αυτή.

# Υλοποιήσεις μέσω ADTStack: ADTQueue
# Για να λειτουργήσουν χρειάζονται και μια υλοποίηση του ADTStack. Χρησιμοποιούμε την UsingADTList/ADTStack.o,
# η οποία με τη σειρά της χρειάζεται μια υλοποίηση του ADTList (χρησιμοποιούμε την UsingLinkedList/ADTList.o).
#
UsingADTStack_ADTQueue_test_OBJS = ADTQueue_test.o $(MODULES)/UsingADTStack/ADTQueue.o $(MODULES)/UsingADTList/ADTStack.o $(MODULES)/UsingLinkedList/ADTList.o $(MODULES)/UsingDynamicArray/ADTVector.o 

# To test που προσθεσα για τo ADTQueue_alt
UsingADTStack_ADTQueue_alt_test_OBJS = ADTQueue_test.o $(MODULES)/UsingADTStack/ADTQueue_alt.o $(MODULES)/UsingADTList/ADTStack.o $(MODULES)/UsingLinkedList/ADTList.o

# Υλοποιήσεις μέσω ADTList: ADTQueue
# Πέρα από το .o της κάθε υλοποίησης (πχ ADTStack.o), χρειάζεται και μια υλοποίηση του ίδιου του ADTList!
#
UsingADTList_ADTQueue_test_OBJS = ADTQueue_test.o $(MODULES)/UsingADTList/ADTQueue.o $(MODULES)/UsingLinkedList/ADTList.o

# Υλοποιήσεις μέσω BinarySearchTree: ADTSet
#
UsingBinarySearchTree_ADTSet_test_OBJS = ADTSet_test.o $(MODULES)/UsingBinarySearchTree/ADTSet.o $(MODULES)/UsingDynamicArray/ADTVector.o

# Υλοποιήσεις μεσω AVL
#
#UsingAVL_ADTSet_test_OBJS = ADTSet_test.o $(MODULES)/UsingLazyBST/ADTSet.o $(MODULES)/UsingDynamicArray/ADTVector.o

# Υλοποιήσεις μεσω B-Tree: ADTSet (εγινε προσθηκη)
#
#UsingBTree_ADTSet_test_OBJS = ADTSet_test.o $(MODULES)/UsingBTree/ADTSet.o $(MODULES)/UsingDynamicArray/ADTVector.o

# Ο βασικός κορμός του Makefile
include ../common.mk