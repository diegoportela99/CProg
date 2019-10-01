#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 500

/*******************************************************************************
* Hashmap for Substation Systems Database
* Names: Owen Dowley, Michael Lardner, Ray Robinson, Diego Portela, Jacob Elali
* Student IDs: 13234505, 12893602, 12616292, 13286378, 13372937
* Description: Hashmap Data Structure
*******************************************************************************/

struct telemetry_point
{
   /*the location of the substation*/
   char location[500];
   /*Unique code for each piece of equipment as an identifier*/
   char desig[500];
   /*What type of equipment, eg. Transformer, circuit breaker*/
   char plant[500];
   /*What container does this equipment belong to, eg.voltage level*/
   char network[500];
   /*The specific point which we log, eg. oil temperature*/
   char quantity[500];
   /*The communications protocol used to talk to equip*/
   char protocol[500];
   /*The channel number of the comms module talking to equip*/
   int number;
   /*The module which holds the channels address*/
   int address;
   /*What type of signal is on the module, eg Dig, Ana*/
   char moduletype[500];
   /*=1 if communicating ok when the CSV was saved, =0 if failed*/
   int failed;
   /*Specifies whether the point was online when the CSV was saved,
	(we can set certain points offline if they are causing problems
	or not required to avoid alarms, events etc.)*/
   int online;
   /*Same as failed, not sure why we use this at work*/
   int faulty;
   /* Out of service */
   int oos;
};

struct DataItem
{
   struct telemetry_point *data;
   int key;
};

struct DataItem *hashArray[SIZE];
struct DataItem *dummyItem;
struct DataItem *item;

int hashCode(int key)
{
   return key % SIZE;
}

struct DataItem *search(int key)
{
   /*get the hash*/
   int hashIndex = hashCode(key);

   /*move in array until an empty*/
   while (hashArray[hashIndex] != NULL)
   {

      if (hashArray[hashIndex]->key == key)
         return hashArray[hashIndex];

      /*go to next cell*/
      ++hashIndex;

      /*wrap around the table*/
      hashIndex %= SIZE;
   }

   return NULL;
}

void insert(int key, struct telemetry_point *data)
{

   struct DataItem *item = (struct DataItem *)malloc(sizeof(struct DataItem));
   item->data = data;
   item->key = key;

   /*get the hash*/
   int hashIndex = hashCode(key);

   /*move in array until an empty or deleted cell*/
   while (hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1)
   {
      /*go to next cell*/
      ++hashIndex;

      /*wrap around the table*/
      hashIndex %= SIZE;
   }

   hashArray[hashIndex] = item;
}

struct DataItem *delete (struct DataItem *item)
{
   int key = item->key;

   /*get the hash*/
   int hashIndex = hashCode(key);

   /*move in array until an empty*/
   while (hashArray[hashIndex] != NULL)
   {

      if (hashArray[hashIndex]->key == key)
      {
         struct DataItem *temp = hashArray[hashIndex];

         /*assign a dummy item at deleted position*/
         hashArray[hashIndex] = dummyItem;
         return temp;
      }

      /*go to next cell*/
      ++hashIndex;

      /*wrap around the table*/
      hashIndex %= SIZE;
   }

   return NULL;
}

void display()
{
   int i = 0;

   for (i = 0; i < SIZE; i++)
   {

      if (hashArray[i] != NULL)
         printf(" (%d,%d)", hashArray[i]->key, hashArray[i]->data->address);
      else
         printf(" ~~ ");
   }

   printf("\n");
}

int main()
{
   struct telemetry_point point;
   dummyItem = (struct DataItem *)malloc(sizeof(struct DataItem));
   dummyItem->data = &point;
   dummyItem->key = -1;
   point.address = 10; /*as an example*/

   insert(1, &point);
   insert(2, &point);
   insert(42, &point);
   insert(4, &point);
   insert(23, &point);

   struct telemetry_point point2;
   point2.address = 7; /*as an example*/
   insert(23, &point2);
   insert(18, &point2);
   insert(13, &point2);
   insert(37, &point2);

   display();
   item = search(23);

   while (item != NULL)
   {
      if (item != NULL)
      {
         printf("Element address found: %d\n", item->data->address);
      }
      else
      {
         printf("Element not found\n");
      }

      delete (item);
      item = search(23);
   }
}