# Getting Started

This document well help you get started with `reason-urql`. It picks up right where the README leaves off. It assumes you've followed the installation instructions, but nothing more.

## Setting Up the Client

To get started with `reason-urql`, the first thing you'll want to do is create the `Client`. The `Client` is the core orchestrator of communication with your GraphQL API, handling all outgoing requests and incoming responses. To create a client, simply call the `make` function from the `Client` module.

```reason
open ReasonUrql;

let client = Client.make(~url="https://mygraphqlapi.com/graphql", ());
```

The `client` accepts a few other configuration options, including `fetchOptions` and `exchanges`, but only `url` is required. By default, `reason-urql` will apply `urql`'s `defaultExchanges` if no exchanges are provided; this will include the `fetchExchange` for executing requests, the `cacheExchange` for caching responses from your API, and the `dedupExchange` for deduplicating in-flight requests. It will also apply standard fetch options if no `fetchOptions` argument is provided, using `POST` as the request method and `application/json` as the `Content-Type` header. We'll see later how to work with these options.

## Linking Client with Provider

Once you have your `Client` setup, you'll need to pass it to your `Provider`, which should wrap the root level of your application. This allows the components and hooks to access the `Client` to execute operations.

```reason
open ReasonUrql;

let client = Client.make(~url="https://mygraphqlapi.com/graphql", ());

/* Wrap your application in Provider, passing it the Client as a prop. */
[@react.component]
let make = () =>
  <Provider client><App /></Provider>
```

## Using Your First Hook

Nice, there's only one more step to getting our GraphQL requests executing – using a hook! `reason-urql` comes with three hooks for getting access to your data – `useQuery`, `useMutation`, and `useSubscription`. Let's check out how we might execute a query with `useQuery`.

```reason
open ReasonUrql;
/* Be sure to open the Hooks module to bring necessary types into scope. */
open Hooks;

/* Create a graphql_ppx module with your GraphQL query. */
module DogsQuery = [%graphql
  {|
  query dogs {
    dogs {
      key
      name
      breed
      likes
    }
  }
|}
];

[@react.component]
let make = () => {
  /* Build your request by calling .make on the graphql_ppx module. */
  let request = DogsQuery.make();

  /* Pass the request to useQuery. */
  let ({ response }, executeQuery) = useQuery(~request, ())

  /* Pattern match on the response variant.
  This variant has constructors for Fetching, Data(d), Error(e), and NotFound. */
  switch (response) {
    | Fetching => <LoadingSpinner />
    | Data(d) => {
      Array.map(dog =>
        <div key=dog##key>
          <span> {js|$dog##name $dog##likes|js}->React.string </span>
          <span> dog##breed->React.string <span>
        </div>,
        d##dogs
      )
    }
    | Error(e) =>
      switch (e.networkError) {
      | Some(_exn) => <div> "Network Error"->React.string </div>
      | None => <div> "Other Error"->React.string </div>
      }
    | NotFound => <div> "Something went wrong!"->React.string</div>
  }
}
```

Sweet 😎! We've executed a query with our `useQuery` hook. Notice that we didn't have to write _any_ types to get 💯% type inference and type saftey on the response. We use the `graphql_ppx` module you pass to `useQuery` to ensure that you're using the data returned by your query in a fully type safe way.

## What's Next?

Check out the example in `examples/2-query` to see a more involved example of using `useQuery`, in addition to `reason-urql`'s `Query` component.
