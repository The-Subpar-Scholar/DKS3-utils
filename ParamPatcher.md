# ParamPatcher
This is header file for C++, it  requires my other header file Process.h to function properly. 
This header file allows for the editing of params in DarkSoulsIII. Available are all innate functions or a variation thereof of igromanru's param patcher version 2.05. 
BaseParamClass has NOT been included yet, but will soon be included.

## Create a change (patch a param)

##### 1. param patcher must be initialized
constructer arg is string name of the process window (this is from Process.h)
```cpp
Process DarkSoulsIII = Process("DARK SOULS III");
```
arg is process object from Process.h
```cpp
ParamPatcher::initialize(Process(DarkSoulsIII));
```
##### 2. param edit object must be created
constructor argument is param string name, and the id to edit
```cpp
ParamPatcher::Patch patch = ParamPatcher::Patch("EquipParamGoods", 1234);
```
##### 3. editing
writes to an offset of the specified id, first arg is offset, second is the value to write.
```cpp
patch.patch4Bytes(0x4,20);
```
##### 3.1 there exists a second option:
writes to the offset of every id in a param same as the above method.
```cpp
patch.patchAll4Byte(0x4,20);
```
## Restoring
restores all changes made in an object, however implementation is not static yet so restoring two objects that edit the same thing will only restore to an edited value
```cpp
patch.restore();
```
## Debug
##### print all changes
we sometimes want to make sure that changes have taken effect so we use ***enumerate_changes()***;
prints all changes as address and pre-change bytes
```cpp
enumerate_changes();
```
example:
```
Address : 7FF49F3DC190 | Original Bytes : 51 0D 00 00
Address : 7FF49F3DC210 | Original Bytes : 5C 0D 00 00
Address : 7FF49F3DC290 | Original Bytes : 48 0D 00 00
Address : 7FF49F3DC310 | Original Bytes : 4A 0D 00 00
Address : 7FF49F3DC390 | Original Bytes : 4B 0D 00 00
```
##### print all params
we dont always have a list of all params, so we can use ***enumerate_params()***;
prints list of all params with name and address
```cpp
enumerate_params();
```
##### print id table
each param has ids and we dont always know what they are, so we use ***enumerate_id_table("ParamName")***;
arg is param name string, same as is printed in with enumerate_params().
```cpp
enumerate_id_table("EquipParamWeapon"); 
```
