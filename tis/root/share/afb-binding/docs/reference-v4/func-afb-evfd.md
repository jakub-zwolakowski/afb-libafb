
# Functions of class **afb\_evfd**


## Function afb\_evfd\_create

```C
/**
 * Creates a callback for handling asynchronousely the events associated with
 * the file descriptor 'fd'.
 *
 * @param efd pointer to receive the created evfd result
 * @param fd  the file descriptor to be handled
 * @param events the events for triggerring callbacks (see epoll_ctl)
 * @param handler the callback handler
 * @param closure the closure for the handler
 * @param autounref boolean, if not null the evfd if automatically unreferenced at close
 * @param autoclose boolean, if not null fd if automatically closed when unreferenced
 *
 * @return 0 in case of success or a negative -errno like value
 */
int
afb_evfd_create(
	afb_evfd_t *efd,
	int fd,
	uint32_t events,
	afb_evfd_handler_t handler,
	void *closure,
	int autounref,
	int autoclose
);
```

## Function afb\_evfd\_addref

```C
/**
 * Add one reference to the evfd object 'efd'
 *
 * @param efd the object to reference
 *
 * @return the referenced object
 */
afb_evfd_t
afb_evfd_addref(
	afb_evfd_t efd
);
```

## Function afb\_evfd\_unref

```C
/**
 * Remove one reference to the evfd object 'efd' and
 * destroys the object if not more referenced
 *
 * @param efd the object to unreference
 */
void
afb_evfd_unref(
	afb_evfd_t efd
);
```

## Function afb\_evfd\_get\_fd

```C
/**
 * Retrieve the file descriptor of the evfd object 'efd'
 *
 * @param efd the object to query
 *
 * @return the file descriptor of the object
 */
int
afb_evfd_get_fd(
	afb_evfd_t efd
);
```

## Function afb\_evfd\_get\_events

```C
/**
 * Retrieve the event flags for the evfd object 'efd'
 *
 * @param efd the object to query
 *
 * @return the event flags of the object
 */
uint32_t
afb_evfd_get_events(
	afb_evfd_t efd
);
```

## Function afb\_evfd\_set\_events

```C
/**
 * Set the event flags for the evfd object 'efd'
 *
 * @param efd    the object to query
 * @param events the events flags (see epoll_ctl)
 */
void
afb_evfd_set_events(
	afb_evfd_t efd,
	uint32_t events
);
```
