
# Functions of class **afb\_data\_array**


## Function afb\_data\_array\_addref

```C
/**
 * Increase the count of references of the array 'array'
 * that has 'count' data.
 *
 * @param count the count of data in the array
 * @param array the array of data
 */
void
afb_data_array_addref(
	unsigned count,
	afb_data_t const *array
);
```

## Function afb\_data\_array\_unref

```C
/**
 * Decrease the count of references of the array 'array'
 * that has 'count' data.
 *
 * Call this function when the data is no more used.
 * It destroys the data when the reference count falls to zero.
 *
 * @param count the count of data in the array
 * @param array the array of data
 */
void
afb_data_array_unref(
	unsigned count,
	afb_data_t const *array
);
```

## Function afb\_data\_array\_convert

```C
/**
 * Get a new instances of items of 'array_data' converted to the
 * corresponding types of 'array_type'
 *
 * If a data are returned (no error case), they MUST be released
 * using afb_data_unref.
 *
 * @param count        count of items in the arrays
 * @param array_data   array of original data
 * @param array_type   array of expected types
 * @param array_result array for storing result of conversions
 *
 * @return 0 in case of success or a negative value indication the error.
 */
int
afb_data_array_convert(
	unsigned count,
	afb_data_t const * array_data,
	afb_type_t const * array_type,
	afb_data_t *array_result
);
```
