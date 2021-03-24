# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [4.0.0] - 2021-03-21

This release adds support for several major version upgrades of dependencies, including:

- Support for `urql@2.0.0`.
- Support for `@rescript/react`. `reason-react` is no longer supported moving forward.
- Support for `bs-platform@9.0.0`.

This release also includes a name change. This package will now be published under the `@urql` scope as `@urql/rescript`. To install it, run:

```sh
yarn add @urql/rescript
```

### Changed

- `reason-urql` is now available as `@urql/rescript`. PR by @parkerziegler [here]().
- `@urql/rescript` is now compatible with `urql@2.0.0`, `bs-platform@9.0.0`, and `@rescript/react`. You must upgrade to `@rescript/react` from `reason-react` to use v4.0.0. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/254).
- The `pollInterval` API was removed from all hooks and Client methods, in accordance with the upstream deprecation of this API in `urql@2.0.0`. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/254). More information on this deprecation can be found in [the `urql` CHANGELOG](https://github.com/FormidableLabs/urql/releases/tag/urql%402.0.0).

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v3.4.0...v4.0.0

## [3.4.0] - 2021-01-18

This release adds bindings for two additional ecosystem exchanges:

- [`@urql/exchange-persisted-fetch`](https://github.com/FormidableLabs/urql/tree/main/exchanges/persisted-fetch)
- [`@urql/exchange-refocus`](https://github.com/FormidableLabs/urql/tree/main/exchanges/refocus)

### Added

- Bindings for `@urql/exchange-persisted-fetch` and `@urql/exchange-refocus` were added. Consumers of `reason-urql` will need to install `@urql/exchange-persisted-fetch` to call `Client.Exchanges.persistedFetchExchange` and `@urql/exchange-refocus` to call `Client.Exchanges.refocusExchange`. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/251).

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v3.3.0...v3.4.0

## [3.3.0] - 2021-01-05

This release adds bindings for two additional ecosystem exchanges:

- [`@urql/exchange-retry`](https://github.com/FormidableLabs/urql/tree/main/exchanges/retry)
- [`@urql/exchange-request-policy`](https://github.com/FormidableLabs/urql/tree/main/exchanges/request-policy)

### Added

- Bindings for `@urql/exchange-retry` and `@urql/exchange-request-policy` were added. Consumers of `reason-urql` will need to install `@urql/exchange-retry` to call `Client.Exchanges.retryExchange` and `@urql/exchange-request-policy` to call `Client.Exchanges.requestPolicyExchange`. PRs by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/244) and [here](https://github.com/FormidableLabs/reason-urql/pull/247).

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v3.2.0...v3.3.0

## [3.2.0] - 2020-12-20

This release adds a thin binding for `@urql/exchange-multipart-fetch`.

### Added

- A thin binding for `@urql/exchange-multipart-fetch` was added. Consumers of `reason-urql` will need to install `@urql/exchange-multipart-fetch` to call `Client.Exchanges.multipartFetchExchange`. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/242). Fixes [#169](https://github.com/FormidableLabs/reason-urql/issues/169).

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v3.1.0...v3.2.0

## [3.1.0] - 2020-12-09

This release migrates our internal syntax from Reason to ReScript. It also revises our binding of `CombinedError` to be compatible with ReScript, which doesn't support binding JS classes to OCaml classes as BuckleScript once did. The experience for end users should be no different.

### Changed

- The codebase is now written in [ReScript](https://rescript-lang.org/). PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/231).
- The binding to `CombinedError` is now a record rather than an OCaml class being bound to a JS class. This change was made to both increase simplicity and to prepare for the BuckleScript to ReScript migration. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/230).

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v3.0.0...v3.1.0

## [3.0.0] - 2020-11-14

This release migrates us to using `@reasonml-community/graphql-ppx` as our GraphQL PPX preprocessor of choice in lieu of `@baransu/graphql_ppx_re`. It also converts `urql` to a `peerDependency` of the library rather than bundling `urql` as a direct dependency. Finally, this release includes support for BuckleScript / ReScript > 8.0.0.

### Changed

- `urql`, along with its own `peerDependencies`, should be installed alongside `reason-urql`, which no longer bundles `urql` as a direct dependency. See updated installation instructions in the README. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/224).
- Users should install `@reasonml-community/graphql-ppx` as a `peerDependency` of `reason-urql`.
- Users depending on `bs-platform>=8.0.0` will need to specify a resolution for `wonka` v5 using [yarn resolutions](https://classic.yarnpkg.com/en/docs/selective-version-resolutions/). See updated installation instructions in the README.
- `Client.execute*` methods underwent four major API changes:

  1. There is no longer a `~request` labelled argument. Instead, the GraphQL operation is passed using `~query` for `Client.executeQuery`, `~mutation` for `Client.executeMutation`, and `~subscription` for `Client.executeSubscription`. This applies to `Client.query`, `Client.mutation`, `Client.subscription`, and `Client.readQuery` as well.
  2. GraphQL operations are passed as [first-class modules](https://dev.realworldocaml.org/first-class-modules.html) to `Client.execute*` methods. This means you pass your entire GraphQL query `module` as a function argument, like so:

  ```reason
  open ReasonUrql;

  module GetAllDogs = [%graphql {|
    query getAllDogs {
      dogs {
        name
        breed
        likes
      }
    }
  |}];

  /* Pass GetAllDogs as a first-class module to Client.executeQuery. */
  Client.executeQuery(~query=(module GetAllDogs), ())
  ```

  3. Variables are passed as the last positional argument to `Client.execute*` methods, if they are required. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/226). We interface with `@reasonml-community/graphql-ppx` to typecheck your `variables` based on the signature of your GraphQL operation. If your query, mutation, or subscription requires variables, pass them as a record in the last position, like so:

  ```reason
  open ReasonUrql;

  module GetDog = [%graphql {|
    query getDog($key: ID!) {
      dog(key: $key) {
        name
        breed
        likes
      }
    }
  |}];

  /* Pass GetDog as a first-class module to Client.executeQuery,
      with required variables as a record in the final position. */
  Client.executeQuery(~query=(module GetDog), {key: "12345"})
  ```

  4. The `response` variant for `Client.execute*` methods was renamed to `operationResponse` and should be referenced from the `Types` module rather than the `Client` module.

- The hooks APIs underwent three major API changes:

  1. There is no longer a `~request` labelled argument. Instead, the GraphQL operation is passed using `~query` for `useQuery`, `~mutation` for `useMutation`, and `~subscription` for `useSubscription`.
  2. GraphQL operations are passed as [first-class modules](https://dev.realworldocaml.org/first-class-modules.html) to each hook. This means you pass your entire GraphQL query `module` as a function argument, like so:

  ```reason
  open ReasonUrql;

  module GetAllDogs = [%graphql {|
    query getAllDogs {
      dogs {
        name
        breed
        likes
      }
    }
  |}];

  [@react.component]
  let make = () => {
    /* Pass GetAllDogs as a first-class module to useQuery. */
    let (Hooks.{response}, _) = Hooks.useQuery(~query=(module GetAllDogs), ());

    /* Return the JSX for your component. */
  }
  ```

  3. Variables are passed as the last positional argument to a hook, if they are required. PR by @amiralies [here](https://github.com/FormidableLabs/reason-urql/pull/225). We interface with `@reasonml-community/graphql-ppx` to typecheck your `variables` based on the signature of your GraphQL operation. If your query, mutation, or subscription requires variables, pass them as a record in the last position, like so:

  ```reason
  module GetDog = [%graphql {|
    query getDog($key: ID!) {
      dog(key: $key) {
        name
        breed
        likes
      }
    }
  |}];

  [@react.component]
  let make = () => {
    /* Pass GetDog as a first-class module to useQuery,
      with required variables as a record in the final position. */
    let (Hooks.{response}, _) = Hooks.useQuery(~query=(module GetDog), {key: "12345"});

    /* Return the JSX for your component. */
  }
  ```

### Removed

- The `useDynamicMutation` hook was removed. `useMutation` now consolidates the former use case for this hook and brings our APIs more inline with how `urql` works.

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v2.1.0...v3.0.0

## [3.0.0-rc.0] - 2020-11-12

This release is the first release candidate for v3.0.0, which will use `@reasonml-community/graphql-ppx` as our PPX preprocessor for GraphQL operations. Breaking changes will be listed in the release notes for the first stable release of v3.0.0.

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v2.1.0...v3.0.0-rc.0

## [2.1.0] - 2020-09-17

This release brings our `urql` dependency up to v1.10.0, at parity with the current state of `urql` 🙌. There are no code changes involved from the previous RC release.

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v2.1.0-rc.0...v2.1.0

## [2.1.0-rc.0] - 2020-09-10

This release fixes some important bugs identified in the v2.0.0 release.

### Fixed

- Properly handle `null` on the `data` field in the GraphQL response. This was a regression from v1.7.0. PR by @parkerziegler and @gaku-sei [here](https://github.com/FormidableLabs/reason-urql/pull/213).
- Revert to using `[@bs.deriving abstract]` to internally create `Partial<OperationContext>` objects. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/216).

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v2.0.0...v2.1.0-rc0

## [2.0.0] - 2020-08-03

This release includes full support for BuckleScript 7 and widely reorganizes `reason-urql` to be more modular and easier to contribute to. Most of the public API has stayed the same, with some exceptions (documented below).

Users who upgrade to v2 should be on BuckleScript 7, as this library relies heavily on [direct record to object compilation](https://reasonml.org/docs/reason-compiler/latest/object#records-as-objects).

### Added

- All hooks and `Client.execute*` methods that accepted a partial operation context argument now accept each key of the `operationContext` record type as an optional function argument. For example, you can now write code like this:

```reason
open ReasonUrql;

/* Define query to execute. */
let subscription = Client.executeQuery(~query, ~requestPolicy=`CacheFirst, ~pollInterval=200, ());
```

`reason-urql` will handle compiling each argument and passing it along to `urql` properly.

- Interface files (`.rei`) were added for all `modules`.
- The `stale` flag is now returned on all results returned by `reason-urql` hooks, which indicates that the result returned by the hook is stale and that another request is being sent in the background. This is particularly useful with the `CacheAndNetwork request policy.

### Changed

- The `response` variant now has 5 constructors – `Fetching`, `Data(d)`, `PartialData(d, e)`, `Error(e)`, and `Empty`. You can read more about each of these [here](./docs/advanced.md#the-response-variant).
- The `UrqlTypes` module is now just `Types`.
- The `Exchanges` module is now a sub-module of the `Client` module. Once `ReasonUrql` is brought into scope it can be refrenced as `Client.Exchanges`.
- `ssrExchange` now accepts `ssrExchangeParams` as its first labeled argument, not `ssrExchangeOpts`. `ssrExchangeParams` is also a record type while `ssrExchangeOpts` was a `[@bs.deriving abstract]`. This brings it more in line with `urql`'s implementation.
- `serializedResult` is now a record type rather than a `[@bs.deriving abstract]`.
- The signature of exchanges has changed to properly support uncurrying syntax.

```reason
type t =
  exchangeInput =>
  (. Wonka.Types.sourceT(UrqlClientTypes.operation)) =>
  Wonka.Types.sourceT(UrqlClientTypes.operationResult);
```

- Local binding of `graphQLError` is now a record type rather than a `[@bs.deriving abstract]` and has its own module `GraphQLError`.

### Removed

- Component bindings for `Query`, `Mutation`, `Subscription`, and `SubscriptionWithHandler` were removed. Just use the hooks APIs!
- `Client` methods for `executeRequestOperation`, `reexecuteOperation`, `createRequestOperation`, and `dispatchOperation` were removed. These are no longer in `urql`'s public facing API.

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v1.7.0...v2.0.0

## [1.7.0] - 2020-04-16

This release migrates our dependency on `urql` to v1.5.1. It also migrates our `devDependency` on `bs-platform` (the BuckleScript compiler), to v7.2.2. While we don't expect this to affect end users still on BuckleScript v6, there may be small bugfixes and optimizations coming in the near future to support this migration.

### Added

- Add support for `Promise`-based methods `query` and `mutation` on the Client. This allows users to interact with Client operation results as `Js.Promise.t` rather than `Wonka.sourceT`. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/157).
- Add bindings for the `useClient` hook. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/159/files).

### Changed

- Migrate local `devDependency` on `bs-platform` to v7.2.2 and in-repo compilation target to `es6`. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/157/files).
- Remove `peerDependency` on `bs-fetch`.

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v1.6.0...v1.7.0

## [1.6.0] - 2020-03-30

This release migrates our dependency on `urql` to v1.4.0. This adds support for setting a default `requestPolicy` on the Client, in addition to setting up polling for your GraphQL queries using the `pollInterval` argument to `useQuery` and the `Query` component.

### Added

- Add option to set a default `requestPolicy` on the Client. PR by @JoviDeCroock [here](https://github.com/FormidableLabs/reason-urql/pull/154).
- Add `pollInterval` argument to `useQuery` hook and `Query` component. PR by @JoviDeCroock [here](https://github.com/FormidableLabs/reason-urql/pull/154).

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v1.5.0...v1.6.0

## [1.5.0] - 2020-01-11

This release migrates our dependency on `urql` to v1.3.0. As such, we now have support for GraphQL extensions, custom `fetch` implementations, and adjustable operation contexts for queries and mutations.

### Added

- Access GraphQL extensions returned by your GraphQL API inside all hooks and componentts. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/141).
- Add support for a custom `fetch` implementation to `clientOptions`. Pr by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/141).
- Support an optional `context` argument to `executeQuery` and `executeMutation`, in addition to an optional initial `context` prop / argument for the `Query` component and `useQuery` hook. PRs by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/141) and [here](https://github.com/FormidableLabs/reason-urql/pull/142).

### Fixed

- Improved documentation on the `reason-urql` API. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/142).

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v1.4.1...v1.5.0

## [1.4.1] - 2019-12-16

This release fixes a small bug with `requestPolicy` in `useQuery`. We weren't properly converting the polymorphic variants passed by a user to their JS string representation, which led to improper request policies being used by the `urql` Client.

### Fixed

- Properly unwrap user-supplied `requestPolicy` to the matching JS string representation. PR by @baransu [here](https://github.com/FormidableLabs/reason-urql/pull/138).

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v1.4.0...v1.4.1

## [1.4.0] - 2019-12-08

This release adds support for a `useDynamicMutation` hook, which allows users to pass variables to the `executeMutation` function returned by the hook _at execution time_, as opposed to at render time as `useMutation` does.

### Added

- Add `useDyanmicMutation` hook to support applying variables when calling `executeMutation`. PR by @Schmavery and @sgrove [here](https://github.com/FormidableLabs/reason-urql/pull/130).

### Fixed

- Restore CI by migrating off of GitHub Actions to Travis CI. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/131).

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v1.3.0...v1.4.0

## [1.3.0] - 2019-11-12

This release migrates us to `bs-platform@6.2.1` and ensures `reason-urql` is compatible for codebases using latest `bs-platform`.

### Added

- Documentation on using `graphql_ppx_re` with `reason-urql`. PR by @huy-nguyen [here](https://github.com/FormidableLabs/reason-urql/pull/119).
- Improved Getting Started documentation. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/125).

### Changed

- Fix type for `pause` argument to `Query` component for `bs-platform@6.2.1`. PR by @gugahoa [here](https://github.com/FormidableLabs/reason-urql/pull/124).
- Migrate to `bs-platform@6.2.1` in source. Fix compiler warnings. Rename `partialOperationContextFn` to `executeQuery`. **This is a breaking change.**

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v1.2.0...v1.3.0

## [1.2.0] - 2019-10-29

This release upgrades our `urql` dependency to 1.2.0 and picks up a handful of small bugfixes introduced in that [release](https://github.com/FormidableLabs/urql/releases/tag/v1.2.0).

### Added

- The `operationContext` object now has a `meta` property on it, containing fields for `source`, `cacheOutcome`, and `networkLatency`. These fields are likely to be deprecated in the next minor release and moved to `bs-urql-devtools-exchange`.

### Changed

- `urql` dependency upgraded to v1.2.0!
- `bs-platform` dependency upgraded to v5.2.1! PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/116).
- Some cmall changes to spurce up the example projects!

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v1.1.0...v1.2.0

## [1.1.0] - 2019-10-17

This release adds experimental support for server-side rendering with the addition of bindings for the `ssrExchange`.

### Added

- Bindings for `ssrExchange` and the `suspense` option on the `Client.make` function. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/111).
- Initial docs for server side rendering support. PR by @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/111).

### Changed

- `urql` dependency upgraded to v1.1.3!

### Fixed

- Minor docs fix by for the `Client` module. PR by @robinweser [here](https://github.com/FormidableLabs/reason-urql/pull/112).
- `bsconfig.json` and `package.json` files from the `examples` directory are no longer published to `npm`.

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v1.0.2...v1.1.0

## [1.0.2] - 2019-10-05

This release makes a small change around the API for `CombinedError`. Specifically, the module type `t` for `CombinedError` now refers to the record exposed by the module rather than the `Js.t` / OCaml class binding `urql`'s native `CombinedError` class. Better documentation for `CombinedError` was also added.

### Added

- More documentation on `CombinedError` in the [API docs](/docs/api.md). PR by @Schmavery and @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/109).

### Changed

- `CombinedError.t` now references the record exposed by the `CombinedError` module containing fields for `message`, `networkError`, `graphQLErrors`, and `message`. PR by @Schmavery and @parkerziegler [here](https://github.com/FormidableLabs/reason-urql/pull/108).

### Fixed

- Capitalization / casing for the `graphQLErrors` field on `CombinedError.t`. Previously this was bound as `graphqlErrors`, which would always result in `None` being returned as `urql` has no `graphqlErrors` field. PR by @Schmavery [here](https://github.com/FormidableLabs/reason-urql/pull/109).

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v1.0.1...v1.0.2

## [1.0.1] - 2019-09-26

This release removes `bs-fetch` as a dependency such that it doesn't conflict with a user's local copy of `bs-fetch`. Since we only use `bs-fetch` for type checking `fetchOptions`, it can safely be included as a `devDependency`. This release also adds a `message` field on the `combinedError` record to provide users access to the raw error string from `urql`.

### Removed

- Dependency on `bs-fetch`. `bs-fetch` is now a `devDependency` for `reason-urql` and can also be installed as a `peerDependency` if you want to use a different version in your app. PR by @parkerziegler and @gugahoa [here](https://github.com/FormidableLabs/reason-urql/pull/104).

### Added

- `message` field on `CombinedError.combinedError`. This represents the raw string `message` returned by the `urql` client for your requests. PR by @Schmavery [here](https://github.com/FormidableLabs/reason-urql/pull/105).

### Diff

https://github.com/FormidableLabs/reason-urql/compare/v1.0.0...v1.0.1

## [1.0.0] - 2019-09-18

This release represents the first stable reason of `reason-urql`. From this point forward we'll be starting to enforce semantic versioning for all changes. Thank you so much to all of the contributors who helped us to achieve this milestone!

### Added

- A [Getting Started](./docs/getting_started.md) guide to help first time users of `reason-urql` get up and running.

### Changed

- The `Error(e)` constructor of the `response` variant has been moved above the `Data(d)` constructor to better match `urql`'s JS API, where uses check for error responses on the GraphQL request before operating on data.
- The `Types` and `Client.Types` `module`s were renamed to `UrqlTypes` and `Client.ClientTypes` to avoid namespace collisions with user-defined `Types` `module`s.

#### Diff

https://github.com/FormidableLabs/reason-urql/compare/v1.0.0-beta.3...v1.0.0

## [1.0.0-beta.3] – Type Inference on Client.execute\* Methods - 2019-08-17

This release adds improved type inference around the `Client`'s `execute*` methods (i.e. `executeQuery`, `executeMutation`, and `executeSubscription`). Responses will now be fully type checked at compile time. This release also pins the bound version of `urql` to 1.0.5 to address a regression that occurs when using `urql` > 1.2.0.

### Changed

- Add proper type inference to `Client.execute*` methods. This now involves passing the full `graphql_ppx` module to the `Client` such that it can grab the `parse` method off of it to properly infer the response. This mimics exactly how the components and hooks work.
- Reorganize types to reduce redundancy and ensure unique generation of `[@bs.deriving abstract]` accessors. This change is mostly internal, but it begins to isolated types used by different `module`s into submodules. For example, rather than all types living in the top-level `Types` module, certain types reserved for particular domains, i.e. the `Client`, live in a submodule i.e. `Client.Types`.

#### Diff

https://github.com/FormidableLabs/reason-urql/compare/v1.0.0-beta.2...v1.0.0-beta.3

## [1.0.0-beta.2] – CombinedError and FetchOption Function - 2019-07-22

This release adds small improvements around `client.fetchOptions` and the `CombinedError` API.

### Changed

- The `FetchFn` variant of the client's `fetchOptions` argument now supports calling the function at runtime of the client rather than invoking it early in order to pass a `fetchOptions` object to `urql`.
- `UrqlCombinedError` is now properly bound using OCaml classes. A helper was added to convert the `CombinedError` `Js.t` to a record type to allow for nicer, typesafe access.

#### Diff

https://github.com/FormidableLabs/reason-urql/compare/v1.0.0-beta.1...v1.0.0-beta.2

## [1.0.0-beta.1] – 🎣 Hooks - 2019-07-06

This release includes support for `urql` hooks, including `useQuery`, `useMutation`, and `useSubscription`. It also includes additions to support proper type inference of GraphQL requests, such that users get compile-time guarantees that they are accessing data in a fully type-safe way.

### Added

- Bindings for `useQuery`, `useMutation`, and `useSubscription`.
- Bindings for `SubscriptionWithHandler`, which is to be used in cases where a custom `handler` function is passed to the `Subscription` component to accumulate subscriptions.
- The top-level `urql` context object, which exposes a `Consumer` and `Provider`, is now exposed to the user.

### Changed

- Migrated all components and examples to latest ReasonReact syntax (`"jsx: 3"`).
- Add proper type inference for hooks and components. This is achieved by passing the entire `graphql_ppx` module to the hooks, which allows them to use the `parse` method to infer the return type of data from the request.

#### Diff

https://github.com/parkerziegler/reason-urql/compare/v1.0.0-beta...v1.0.0-beta.1

## [1.0.0-beta] - 🎉 v1 (Beta) - 2019-05-16

This represents the beta release of v1, which provides support for [`urql` v1](https://github.com/FormidableLabs/urql/tree/v1.0.5).

### Added

- Bindings for `Query`, `Mutation`, and `Subscription` components.
- Bindings for `Client` with support for **exchanges**.
- Bindings for `Provider`.
- Bindings for `urql`'s type interfaces, utils, and helpers.
- Examples for `Query`, `Mutation`, `Subscription`, exposed exchanges, custom exchanges, and imperative `Client` methods.
- First tests for `UrqlClient`.

#### Diff

https://github.com/parkerziegler/reason-urql/compare/v0.1.1...v1.0.0-beta

## [0.1.1] - Coverage 💅 - 2019-01-29

### Added

- Additional tests for `Query`, `Mutation`, `Connect`, and `Client`.

### Fixed

- The `invalidate` cache operation supplied to `Connect` now properly accepts a single, optional labelled argument `~query: Query.urqlQuery`. This will now type check properly thanks to a final positional `unit` parameter in the type definition.

#### Diff

https://github.com/parkerziegler/reason-urql/compare/v0.1.0...v0.1.1

## [0.1.0] – We're Stable(ish)! - 2018-12-07

Welcome to v0.1.0 of `reason-urql`. With this release, we are moving into a stable state and will continue onwards with standard [semantic versioning](https://semver.org/).

### Added

- Automated builds with Travis CI.
- Automated coverage reporting with Coveralls.
- Unit tests for `UrqlQuery`, `UrqlMutation`, `UrqlConnect`, `UrqlClient`, and `UrqlProvider`.
- Codified utilities for making tests more concise – see `TestUtils.re`.
- Zero-config, hot reloading in the example project.
- Small, non-breaking changes to the API.

#### Diff

https://github.com/parkerziegler/reason-urql/compare/v0.1.0-beta...v0.1.0

## [0.1.0-beta] - GraphQL, meet Reason - 2018-10-07

This is the initial beta release of reason-urql! These bindings are currently untested and should not be used in production just yet. Come help us by writing tests, opening issues, and filing PRs. Thanks!
