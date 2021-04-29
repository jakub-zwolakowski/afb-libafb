
# Functions of class **afb\_data**


## Function afb\_data\_is\_valid

```C
/**
 * Checks whether the 'data' is valid or not.
 *
 * @param data the data to check
 *
 * @return 0 if not valid or 1 if valid.
 */
int
afb_data_is_valid(
	afb_data_t data
);
```

## Function afb\_create\_data\_raw

```C
/**
 * Creates a new data instance of the given type with the given values of
 * pointer, size, dispose and closure.
 *
 * A data is defined by the 5 given values defined here:
 *
 *   - type: is the public type of the data (it can be NULL -not recommanded-)
 *   - pointer: a pointer that can be NULL
 *   - size: a size that can be zero but that must be less than 4294967296 (2**32)
 *   - dispose: a function called with closure when data is no more referenced
 *              it can be NULL if nothing has to be done to release resources
 *   - closure: the closure for the dispose callback
 *
 * Once created, the data wraps public values type, pointer and size for its life time.
 * The life time of the data is given by reference counting done using
 * functions @see afb_data_addref and @see afb_data_unref
 *
 * When the data is no more used, it will release the data using dispose if
 * dispose is not null
 *
 * In case of error, if the dispose function is defined, it is called to release
 * the wrapped data. The closure given to the dispose function can be the given
 * pointer or can be any meaningful value.
 *
 * @param data     pointer to the created data
 * @param type     type of the data to created
 * @param pointer  pointer of the data to create
 * @param size     size of the data to create
 * @param dispose  a callback for releasing resources
 * @param closure  a closure for re the callback dispose
 *
 * @return 0 in case of successful subscription or negative value in case of error.
 */
int
afb_create_data_raw(
	afb_data_t *data,
	afb_type_t type,
	const void *buffer,
	size_t size,
	void (*dispose)(void*),
	void *closure
);
```

## Function afb\_create\_data\_alloc

```C
/**
 * Creates a new data instance of the given type by allocating memory
 * of the given size. The allocated memory filled with zeroes.
 *
 * This function is equivalent to (if allocation doesn't fails):
 *
 *    *pointer = calloc(1, size);
 *    return afb_create_data_raw(data, type, *pointer, size, free, *pointer);
 *
 * @param data     pointer to the created data
 * @param type     type of the data to created
 * @param pointer  pointer of the data to create
 * @param size     size of the data to create
 *
 * @return 0 in case of successful subscription or negative value in case of error.
 */
int
afb_create_data_alloc(
	afb_data_t *data,
	afb_type_t type,
	void **pointer,
	size_t size
);
```

## Function afb\_create\_data\_copy

```C
/**
 * Creates a new data instance of the given type by copying the memory
 * given by pointer and size. The memory where data is copied can eventually
 * be shared.
 *
 * This function is equivalent to (except it returns valid code):
 *
 *    if (afb_create_data_alloc(data, type, &ptr, size, 0) >= 0)
 *        memcpy(ptr, pointer, size);
 *
 * @param data     pointer to the created data
 * @param type     type of the data to created
 * @param pointer  pointer of the data to create
 * @param size     size of the data to create
 *
 * @return 0 in case of successful subscription or negative value in case of error.
 */
int
afb_create_data_copy(
	afb_data_t *data,
	afb_type_t type,
	const void *buffer,
	size_t size
);
```

## Function afb\_data\_addref

```C
/**
 * Increases the count of references of 'data'
 *
 * @param data the data
 *
 * @return the data
 */
afb_data_t
afb_data_addref(
	afb_data_t data
);
```

## Function afb\_data\_unref

```C
/**
 * Decreases the count of references of 'data'.
 * Call this function when the data is no more used.
 * It destroys the data when the reference count falls to zero.
 *
 * @param data the data
 */
void
afb_data_unref(
	afb_data_t data
);
```

## Function afb\_data\_convert

```C
/**
 * Gets a new instance of the 'data' converted to the 'type'
 *
 * If a data is returned (no error case), it MUST be released
 * using afb_data_unref.
 *
 * When type is NULL, the function returns the current data
 * with a reference count incremented by one.
 *
 * @param data   the original data
 * @param type   the expected type
 * @param result location for storing the result of the conversion
 *
 * @return 0 in case of success or a negative value indication the error.
 */
int
afb_data_convert(
	afb_data_t data,
	afb_type_t type,
	afb_data_t *result
);
```

## Function afb\_data\_type

```C
/**
 * Gets the type of the data.
 *
 * @param data the data
 *
 * @return the type of the data
 */
afb_type_t
afb_data_type(
	afb_data_t data
);
```

## Function afb\_data\_get\_mutable

```C
/**
 * Gets a mutable pointer to the data and also its size
 *
 * @param data the data
 * @param pointer if not NULL address where to store the pointer
 * @param size if not NULL address where to store the size
 *
 * @return 0 in case of success or -1 in case of error
 */
int
afb_data_get_mutable(
	afb_data_t data,
	void **pointer,
	size_t *size
);
```

## Function afb\_data\_get\_constant

```C
/**
 * Gets a mutable pointer to the data.
 * Getting a mutable pointer has the effect of
 * notifying that the data changed.
 *
 * @param data the data
 *
 * @return the pointer (can be NULL)
 */
int
afb_data_get_constant(
	afb_data_t data,
	void **pointer,
	size_t *size
);
```

## Function afb\_data\_size

```C
/**
 * Gets the size of the data
 *
 * @param data the data
 *
 * @return the size of the data, can be zero
 */
size_t
afb_data_size(
	afb_data_t data
);
```

## Function afb\_data\_ro\_pointer

```C
/**
 * Gets a read only pointer to the data
 *
 * @param data the data
 *
 * @return the buffer (can be NULL)
 */
void *
afb_data_ro_pointer(
	afb_data_t data
);
```

## Function afb\_data\_rw\_pointer

```C
/**
 * Gets a read/write pointer to the data.
 * Returns NULL if the data is constant.
 *
 * @param data the data
 *
 * @return the buffer (can be NULL)
 */
void *
afb_data_rw_pointer(
	afb_data_t data
);
```

## Function afb\_data\_notify\_changed

```C
/**
 * Notifies that the data changed and that any of its conversions are not
 * more valid.
 *
 * @param data the data that changed
 */
void
afb_data_notify_changed(
	afb_data_t data
);
```

## Function afb\_data\_is\_volatile

```C
/**
 * Tests if the data is volatile. Conversions of volatile data are never cached.
 *
 * @param data the data to test
 *
 * @return 1 if the data is volatile or 0 otherwise
 */
int
afb_data_is_volatile(
	afb_data_t data
);
```

## Function afb\_data\_set\_volatile

```C
/**
 * Makes the data volatile
 *
 * @param data the data to set
 */
void
afb_data_set_volatile(
	afb_data_t data
);
```

## Function afb\_data\_set\_not\_volatile

```C
/**
 * Makes the data not volatile
 *
 * @param data the data to set
 */
void
afb_data_set_not_volatile(
	afb_data_t data
);
```

## Function afb\_data\_is\_constant

```C
/**
 * Tests if the data is constant.
 *
 * @param data the data to test
 *
 * @return 1 if the data is constant or 0 otherwise
 */
int
afb_data_is_constant(
	afb_data_t data
);
```

## Function afb\_data\_set\_constant

```C
/**
 * Makes the data constant
 *
 * @param data the data to set
 */
void
afb_data_set_constant(
	afb_data_t data
);
```

## Function afb\_data\_set\_not\_constant

```C
/**
 * Makes the data not constant
 *
 * @param data the data to set
 */
void
afb_data_set_not_constant(
	afb_data_t data
);
```

## Function afb\_data\_lock\_read

```C
/**
 * Locks the data for read, blocks the current thread
 * until the data is available for reading.
 *
 * The data MUST be unlocked afterward using 'afb_data_unlock'
 *
 * @param data the data to lock for read
 */
void
afb_data_lock_read(
	afb_data_t data
);
```

## Function afb\_data\_try\_lock\_read

```C
/**
 * Try to locks the data for read. Always return immediately
 * with a status indicating whether the data has been locked
 * for read or whether it wasn't possible to lock it for read.
 *
 * If the lock was successful, the data MUST be unlocked
 * afterward using 'afb_data_unlock'.
 *
 * @param data the data to lock for read
 *
 * @return 0 in case of success or a negative -errno status if not locked
 */
int
afb_data_try_lock_read(
	afb_data_t data
);
```

## Function afb\_data\_lock\_write

```C
/**
 * Locks the data for write, blocks the current thread
 * until the data is available for writing.
 *
 * The data MUST be unlocked afterward using 'afb_data_unlock'
 *
 * @param data the data to lock for write
 */
void
afb_data_lock_write(
	afb_data_t data
);
```

## Function afb\_data\_try\_lock\_write

```C
/**
 * Try to locks the data for write. Always return immediately
 * with a status indicating whether the data has been locked
 * for write or whether it wasn't possible to lock it for write.
 *
 * If the lock was successful, the data MUST be unlocked
 * afterward using 'afb_data_unlock'.
 *
 * @param data the data to lock for write
 *
 * @return 0 in case of success or a negative -errno status if not locked
 */
int
afb_data_try_lock_write(
	afb_data_t data
);
```

## Function afb\_data\_unlock

```C
/**
 * Unlock a locked data. It is an error to unlock a data that
 * the current thread doesn't hold locked.
 *
 * @param data the data to unlock
 */
void
afb_data_unlock(
	afb_data_t data
);
```

## Function afb\_data\_update

```C
/**
 * Update the value of the given data with the given value
 *
 * @param data the data to be changed, must be mutable
 * @param value the value to set to data, possibly with convertion
 *
 * @return 0 on success or a negative -errno like value
 */
int
afb_data_update(
	afb_data_t data,
	afb_data_t value
);
```

## Function afb\_data\_assign

```C
/**
 * Replace 'data' with the given 'value', taking care to unreference
 * the data.
 *
 * @param data address of the data to assign
 * @param value value to assign to the data
 */
void
afb_data_assign(
	afb_data_t *data,
	afb_data_t value
);
```

## Function afb\_data\_dependency\_add

```C
/**
 * Add one dependency from a data to an other. This ensures
 * that pointers of the data @p to_data can be safely referenced
 * by the data @p from_data
 *
 * @param from_data the data that depends of @p to_data
 * @param to_data that from wich depends @p from_data
 *
 * @return 0 in case of success or a negative number
 */
int
afb_data_dependency_add(
	afb_data_t from_data,
	afb_data_t to_data
);
```

## Function afb\_data\_dependency\_sub

```C
/**
 * Remove one dependency from a data to an other
 *
 * @param from_data the data that depends of @p to_data
 * @param to_data that from wich depends @p from_data
 *
 * @return 0 in case of success or a negative number
 */
int
afb_data_dependency_sub(
	afb_data_t from_data,
	afb_data_t to_data
);
```

## Function afb\_data\_dependency\_drop\_all

```C
/**
 * Remove all dependencies of the @p data
 *
 * @param data the data whose dependencies can be cleaned
 */
void
afb_data_dependency_drop_all(
	afb_data_t data
);
```
