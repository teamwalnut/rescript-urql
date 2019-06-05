# Getting started with reason-urql

## Setting up client

First we set up the client:

```reasonml
let client = Client.make(~url="https://api.github.com/graphql", ());
```

We then pipe that client into the provider:

```reasonml
    <Provider value=client>
        ...children
    </Provder>
```

This provides the client to all children.

At this point 