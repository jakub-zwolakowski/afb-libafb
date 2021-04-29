
# Functions of class **afb\_event**


## Function afb\_event\_is\_valid

```C
/**
 * Checks whether the 'event' is valid or not.
 *
 * @param event the event to check
 *
 * @return 0 if not valid or 1 if valid.
 */
int
afb_event_is_valid(
	afb_event_t event
);
```

## Function afb\_event\_addref

```C
/**
 * Increases the count of references to 'event'
 *
 * @param event the event
 *
 * @return the event
 */
afb_event_t
afb_event_addref(
	afb_event_t event
);
```

## Function afb\_event\_unref

```C
/**
 * Decrease the count of references to 'event'.
 * Call this function when the evenid is no more used.
 * It destroys the event when the reference count falls to zero.
 *
 * @param event the event
 */
void
afb_event_unref(
	afb_event_t event
);
```

## Function afb\_event\_name

```C
/**
 * Gets the name associated to 'event'.
 *
 * @param event the event whose name is requested
 *
 * @return the name of the event
 *
 * The returned name can be used until call to 'afb_event_unref'.
 * It shouldn't be freed.
 */
const char *
afb_event_name(
	afb_event_t event
);
```

## Function afb\_event\_push

```C
/**
 * Pushes an event of 'event' with the 'data' to its observers.
 * 'data' can be NULL.
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for 'params'.
 * Thus, in the case where 'params' should remain available after
 * the function returns, the function 'afb_data_addref' shall be used.
 *
 * @param event    the event to push
 * @param nparams  count of parameter data
 * @param params   array of parameter data (can be NULL when nparams is zero)
 *
 * @Return
 *   *  1 if at least one client listen for the event
 *   *  0 if no more client listen for the event
 *   * -1 in case of error (the event can't be delivered)
 */
int
afb_event_push(
	afb_event_t event,
	unsigned nparams,
	afb_data_t const params[]
);
```

## Function afb\_event\_broadcast

```C
/**
 * Broadcasts widely an event of 'event' with the 'data.
 * 'data' can be NULL.
 *
 * NOTE: For convenience, the function calls 'afb_data_unref' for 'params'.
 * Thus, in the case where 'params' should remain available after
 * the function returns, the function 'afb_data_addref' shall be used.
 *
 * @param event    the event to broadcast
 * @param nparams  count of parameter data
 * @param params   array of parameter data (can be NULL when nparams is zero)
 *
 * @return 0 in case of success or -1 in case of error
 */
int
afb_event_broadcast(
	afb_event_t event,
	unsigned nparams,
	afb_data_t const params[]
);
```
