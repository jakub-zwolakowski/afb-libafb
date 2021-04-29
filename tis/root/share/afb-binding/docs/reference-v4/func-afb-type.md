
# Functions of class **afb\_type**


## Function afb\_type\_lookup

```C
/**
 * Lookup for an existing type
 *
 * @param type pointer to the type returned if found
 * @param name name of the searched type
 *
 * @return 0 in case of success or a negative error code
 */
int
afb_type_lookup(
	afb_type_t *type,
	const char *name
);
```

## Function afb\_type\_register

```C
/**
 * Register a type
 *
 * @param type pointer to the returned created type
 * @param name name of the type to be created
 * @param flags flags of the created type: a or combination of
 *                 - Afb_Type_Flags_Shareable:
 *                      Data of that type can be shared through memory
 *                 - Afb_Type_Flags_Streamable:
 *                      Data of that type can be streamed
 *                 - Afb_Type_Flags_Opaque:
 *                      Data of that type can be opacified.
 *
 * @return 0 in case of success or a negative error code
 */
int
afb_type_register(
	afb_type_t *type,
	const char *name,
	afb_type_flags_t flags
);
```

## Function afb\_type\_name

```C
/**
 * Get the name of a type
 *
 * @param type the type whose name is queried
 *
 * @return the name of the type
 */
const char *
afb_type_name(
	afb_type_t type
);
```

## Function afb\_type\_set\_family

```C
/**
 * Set the family of the type. An instance of a type naturally converts
 * to an instance of its family.
 *
 * @param type the type whose family is to update
 * @param family the family to set to the type
 *
 * @return 0 on success or a negative -errno like error code
 */
int
afb_type_set_family(
	afb_type_t type,
	afb_type_t family
);
```

## Function afb\_type\_add\_convert\_to

```C
/**
 * Add a convertion routine to a given type
 *
 * @param type the reference type
 * @param to_type the type to convert to
 * @param converter the converter routine
 * @param closure the closure for the converter
 *
 * @return 0 in case of success or a negative error code
 */
int
afb_type_add_convert_to(
	afb_type_t type,
	afb_type_t to_type,
	afb_type_converter_t converter,
	void *closure
);
```

## Function afb\_type\_add\_convert\_from

```C
/**
 * Add a convertion routine from a given type
 *
 * @param type the reference type
 * @param from_type the type to convert from
 * @param converter the converter routine
 * @param closure the closure for the converter
 *
 * @return 0 in case of success or a negative error code
 */
int
afb_type_add_convert_from(
	afb_type_t type,
	afb_type_t from_type,
	afb_type_converter_t converter,
	void *closure
);
```

## Function afb\_type\_add\_update\_to

```C
/**
 * Add an update routine to a given type
 *
 * @param type the reference type
 * @param to_type the type to update to
 * @param updater the updater routine
 * @param closure the closure for the converter
 *
 * @return 0 in case of success or a negative error code
 */
int
afb_type_add_update_to(
	afb_type_t type,
	afb_type_t to_type,
	afb_type_updater_t updater,
	void *closure
);
```

## Function afb\_type\_add\_update\_from

```C
/**
 * Add an update routine from a given type
 *
 * @param type the reference type
 * @param from_type the type to update from
 * @param updater the updater routine
 * @param closure the closure for the converter
 *
 * @return 0 in case of success or a negative error code
 */
int
afb_type_add_update_from(
	afb_type_t type,
	afb_type_t from_type,
	afb_type_updater_t updater,
	void *closure
);
```
