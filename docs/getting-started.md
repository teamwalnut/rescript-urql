# Getting Started

This document well help you get started with `reason-urql`. It picks up right where the README leaves off. It assumes you've followed the installation instructions, but nothing more.

## Setting Up the Client

To get started with `reason-urql`, the first thing you'll want to do is create your client. Your client is the core orchestrator of communication with your GraphQL API, handling all outgoing requests and incoming responses. To create a client, simply call the `make` function from the `Client` module.

```reason
open ReasonUrql;

let client = Client.make(~url="https://mygraphqlapi.com/graphql", ());
```

The `client` accepts a few other configuration options, including `fetchOptions` and `exchanges`, but only `url` is required. By default, `reason-urql` will apply `urql`'s `defaultExchanges` if no exchanges are provided; this will include the `fetchExchange` for executing requests, the `cacheExchange` for caching responses from your API, and the `dedupExchange` for deduplicating in-flight requests. It will also apply standard fetch options if no `fetchOptions` argument is provided, using `POST` as the request method and `application/json` as the `Content-Type` header. We'll see later how to work with these options.

## Linking Client with Provider

Once you have your `Client` setup, you'll need to pass it to your `Provider`, which should wrap the root level of your application. This allows `reason-urql`'s components and hooks to access the `Client` to execute operations lower down in your React tree.

```reason
open ReasonUrql;

let client = Client.make(~url="https://mygraphqlapi.com/graphql", ());

/* Wrap your application in Provider, passing it the client as a prop. */
[@react.component]
let make = () =>
  <Provider value=client><App /></Provider>
```

## Using Your First Hook

Nice, there's only one more step to getting our GraphQL requests executing – using a hook! `reason-urql` comes with three hooks for getting access to your data – `useQuery`, `useMutation`, and `useSubscription`. Let's check out how we might execute a query with `useQuery`.

```reason
open ReasonUrql;

/* Create a module with your GraphQL query. */
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
  /* Build your request by calling .make on your query. */
  let request = DogsQuery.make();

  /* Pass the request to useQuery. */
  let (Hooks.{ response }, executeQuery) = Hooks.useQuery(~request, ());

  /* Pattern match on the response variant.
  This variant has constructors for Init, Fetching, Data(d), Error(e), and Empty. */
  switch (response) {
    | Init => <div> "Init"->React.string </div>
    | Fetching => <LoadingSpinner />
    | Data(d) => {
      Array.map(dog =>
        <div key=dog##key>
          <span> {j|$dog##name $dog##likes|j}->React.string </span>
          <span> dog##breed->React.string <span>
        </div>,
        d##dogs
      )
    }
    | Error(e) =>
      switch (e.networkError) {
      | Some(_e) => <div> "Network Error"->React.string </div>
      | None => <div> "Other Error"->React.string </div>
      }
    | Empty => <div> "Empty"->React.string </div>
  }
}
```

Sweet 😎! We've executed a query with our `useQuery` hook. Notice that we didn't have to write _any_ types to get 💯% type inference and type safety on the response. We use type information included in the query module you pass to `useQuery` to ensure that you're using the data returned by your query in a fully safe way.

## Can I See an Example?

Check out the example in `examples/2-query` to see a more involved example of using `useQuery`, in addition to `reason-urql`'s `Query` component.

## Writing Your First Mutation

Awesome, so we've written our first query with `reason-urql`. What about mutations?

Fortunately, writing mutations in `reason-urql` is just as easy as writing queries – just use the `useMutation` hook.

```reason
open ReasonUrql;

/* Create a module with your GraphQL mutation. */
module LikeDogMutation = [%graphql
    {|
    mutation likeDog($key: ID!) {
      likeDog(key: $key) {
        likes
        name
        breed
      }
    }
  |}
  ];

[@react.component]
let make = (~key: string) => {
  /* Build your request by calling .make on your mutation, passing variables as labeled arguments. */
  let request = LikeDogMutation.make(~key, ());

  /* Pass the request to useMutation. */
  let (_, executeMutation) = Hooks.useMutation(~request, ());

  <button onClick={_e => executeMutation() |> ignore}>
      "Execute the Mutation (and Reward a Good Dog)"->React.string
  </button>
}
```

Great – we've successfully executed a mutation to like a dog! Existing queries that reference the mutated data will be notified that data has changed, meaning you don't need to think at all about refetching your data – it's all updated for you by `reason-urql`.

`useMutation` returns a two-dimensional tuple, containing `(result, executeMutation)`. `result` contains the `response` variant, which allows you to pattern match on the API response from your mutation. For example, if you wanted to show different UI when your mutation was `Fetching`, or if there was an `Error` you can do something like the following:

```reason
open ReasonUrql;

[@react.component]
let make = (~key: string) => {
  /* Build your request by calling .make on your mutation, passing variables as labeled arguments. */
  let request = LikeDogMutation.make(~key, ());

  /* Pass the request to useMutation. */
  let (Hooks.{ response }, executeMutation) = Hooks.useMutation(~request, ());

  let button = React.useMemo1(() =>
    <button onClick={_e => executeMutation() |> ignore}>
        "Execute the Mutation (and Reward a Good Dog)"->React.string
    </button>,
    [|executeMutation|]
  );

  switch (response) {
    | Fetching =>
      /* If the mutation is still executing, display a LoadingSpinner on top of our button. */
      <LoadingSpinner>
        button
      <LoadingSpinner>
    | Data(_d) => button
    /* If an error is encountered when executing the mutation, show some error UI to the user. */
    | Error(e) =>
      <MyErrorComponent message=e.message>
        button
      </MyErrorComponent>
    | _ => React.null
  }
}
```

## Can I See an Example?

Check out the example in `examples/3-mutation` to see a more involved example of using `useMutation`, in addition to `reason-urql`'s `Mutation` component.
