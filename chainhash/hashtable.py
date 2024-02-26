"""
description: Hash Table Using Chaining for CS 141 Lab
file: hashtable.py
language: python3

author: as@cs.rit.edu Amar Saric 
"""

from rit_object import *

class HashTable(rit_object):
    """
       The HashTable data structure contains a collection of values
       where each value is located by a hashable key.
       No two values may have the same key, but more than one
       key may have the same value.
       table is the list holding the hash table
       size is the number of entrys in occupying the hashtable
    """
    
    __slots__ = ('table', 'size') 
    _types = (list, int)

def createHashTable(capacity=100):
    """
    createHashTable: NatNum? -> HashTable
    
    Returns a hash table  as a list of lists. 
    """
    
    return HashTable ([[] for _ in range(capacity)], 0)


def LOAD_FACTOR():
    return 0.75

def resizeHashTable(hTable):
    """
    HashTableToStr: HashTable -> None
    
    Doubles the size of the table.
    """

    #COMPLETE THIS FUNCTION

    return None    


def HashTableToStr(hTable):
    """
    HashTableToStr: HashTable -> String
    """
    
    txt = ''
    ltext = 0
    for lst in hTable.table:
        for entry in lst:
            s = EntryToStr(entry) + ' '
            ltext += len(s)
            txt += s
            if ltext > 51:
                txt += '\n'
                ltext = 0                
    return txt


class Entry(rit_object):
    """
       A class used to hold key/value pairs.
    """
    
    __slots__ = ("key", "value")
    _types = (object, object)


def EntryToStr(entry):
    """
    EntryToStr: Entry -> String
    
    Returns the string representation of the entry.
    """
    
    return "(" + str(entry.key) + ", " + str(entry.value) + ")"


def hash_function(val, n):
    """
    hash_function: K NatNum -> NatNum
    
    Computes a hash of the val string that is in [0 ... n).
    """
    
    #COMPLETE THIS FUNCTION
    
    return 0

def imbalance(hTable):
    """
    keys: HashTable(K, V) -> Num
    
    Computes the imbalance of the hashtable.
    """
    
    #COMPLETE THIS FUNCTION
    
    return 0

def keys(hTable):
    """
    keys: HashTable(K, V) -> List(K)
    
    Returns a list of keys in the given hashTable.
    """
    
    keys_lst = []
    for lst in hTable.table:
        for entry in lst:
            keys_lst += [entry.key]
    
    return keys_lst

def has(hTable, key):
    """
    has: HashTable(K, V) K -> Boolean
    
    Returns True iff hTable has an entry with the given key.
    """
    
    index = hash_function(key, len(hTable.table))
    for entry in hTable.table[index]:
        if entry.key == key:
            return True
    return False 
    

def put(hTable, key, value):
    """
    put: HashTable(K, V) K V -> Boolean
    
    Using the given hash table, set the given key to the
    given value. If the key already exists, the given value
    will replace the previous one already in the table.
    
    The table is resized if the load factor of 0.75 is exceeded.
    """
    
    #COMPLETE THIS FUNCTION

    return True

def get(hTable, key):
    """
    get: HashTable(K, V) K -> V
    
    Return the value associated with the given key in
    the hash table.

    Precondition: has(hTable, key)
    """
    
    #COMPLETE THIS FUNCTION
        
    return None 