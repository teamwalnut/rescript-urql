# reason-urql

[![npm](https://img.shields.io/npm/v/reason-urql.svg)](https://www.npmjs.com/package/reason-urql)
[![All Contributors](https://img.shields.io/badge/all_contributors-9-orange.svg?style=flat-square)](#contributors)
[![Build Status](https://github.com/FormidableLabs/reason-urql/workflows/reason-urql%20CI/badge.svg)](https://github.com/FormidableLabs/reason-urql/actions)
[![Maintenance Status][maintenance-image]](#maintenance-status)
[![Spectrum](https://withspectrum.github.io/badge/badge.svg)](https://spectrum.chat/urql)

Reason bindings for Formidable's Universal React Query Library, [`urql`](https://github.com/FormidableLabs/urql).

## ✨Features

- ⚛️ A fully featured GraphQL client for ReasonReact.
- ✅ Compile time type and schema validation.
- ⚙️ Customizable behavior via `exchanges`.
- 🎣 Support for `useQuery`, `useMutation`, and `useSubscription` hooks!

`reason-urql` is a GraphQL client for ReasonReact, allowing you to hook up your components to queries, mutations, and subscriptions. It provides bindings to `urql` that allow you to use the API in Reason, with the benefits of a sound type system, blazing fast compilation, and opportunities for guided customization.

## 📋 Documentation

- [Getting Started](/docs/getting_started.md)
- [API](/docs/api.md)

## 💾 Installation

#### 1. Install `reason-urql`.

```sh
yarn add reason-urql
```

#### 2. Add `graphql_ppx`.

This project uses [`graphql_ppx`](https://github.com/mhallin/graphql_ppx) to type check your GraphQL queries, mutations, and subscriptions **at compile time**. You'll need to add it as a dev dependency.

```sh
yarn add graphql_ppx --dev
```

#### 3. Update `bsconfig.json`.

Add `reason-urql` to your `bs-dependencies` and `graphql_ppx/ppx` to your `ppx_flags` in `bsconfig.json`.

```json
{
  "bs-dependencies": ["reason-urql"],
  "ppx-flags": ["graphql_ppx/ppx"]
}
```

#### 4. Send an introspection query to your API.

Finally, you'll need to send an introspection query to your GraphQl API. This allows `graphql_ppx` to generate a `graphql_schema.json` at the root of your project that it can use to type check your queries. **You should check this file into version control** and keep it updated as your API changes. To do this:

```sh
yarn send-introspection-query <your_graphql_endpoint>
```

Simply re-run this script at anytime to regenerate the `graphql_schema.json` file. See the [docs for `graphql_ppx`](https://github.com/mhallin/graphql_ppx) for more assistance.

### Older Versions

Before version 1.0.0, `reason-urql` listed `urql` as a peer dependency. If using `v0.1.1` or earlier of `reason-urql`, make sure to install the correct version of `urql`.

```sh
yarn add reason-urql@0.1.1 urql@0.2.2
```

## 💻 Example Projects

`reason-urql` has a nice set of examples showing how to use the basic components and APIs to get the most out of GraphQL and Reason in your app – check them out in the `/examples` folder. To run any of the examples, follow these simple steps.

```sh
# 1. Navigate into the example of choice.
cd examples/1-execute-query-mutation

# 2. Install dependencies.
yarn

# 3. In one terminal, compile the source in watch mode.
yarn start

# 4. In another terminal, start the demo app server.
yarn start:demo
```

The example will start up at `http://localhost:8080`. Edit the example freely to watch changes take effect.

### Editing `reason-urql` source files

If developing on the main `reason-urql` source files (i.e. anything in `/src/`) and you want to test the changes in one of the examples, you'll need to do the following:

```sh
# Save your changes to source, then take the following steps.

# 1. Clean any artifacts from previous builds.
yarn clean

# 2. Rebuild the source.
yarn build

# 3. Clean example build and reinstall dependencies.
cd examples/2-query
yarn clean
yarn
```

Since we are `link`ing the examples' dependency on `reason-urql` to the `src` directory, it's important to clean builds between changes to prevent any stale or erroneous artifacts.

## Getting Involved

This project is currently under active development. Please help out by [opening an issue](https://github.com/FormidableLabs/reason-urql/issues) or [filing a PR](https://github.com/FormidableLabs/reason-urql/pulls).

## Contributors

This project follows the [all contributors spec](https://github.com/kentcdodds/all-contributors). Thanks to these wonderful folks for contributing ([Emoji Key](https://github.com/kentcdodds/all-contributors#emoji-key)):

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore -->
<table>
  <tr>
    <td align="center"><a href="http://parkerziegler.com/"><img src="https://avatars0.githubusercontent.com/u/19421190?v=4" width="100px;" alt="Parker Ziegler"/><br /><sub><b>Parker Ziegler</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=parkerziegler" title="Code">💻</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=parkerziegler" title="Documentation">📖</a> <a href="#review-parkerziegler" title="Reviewed Pull Requests">👀</a> <a href="#ideas-parkerziegler" title="Ideas, Planning, & Feedback">🤔</a></td>
    <td align="center"><a href="https://khoanguyen.me"><img src="https://avatars2.githubusercontent.com/u/3049054?v=4" width="100px;" alt="Khoa Nguyen"/><br /><sub><b>Khoa Nguyen</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=thangngoc89" title="Code">💻</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=thangngoc89" title="Documentation">📖</a></td>
    <td align="center"><a href="https://twitter.com/_philpl"><img src="https://avatars0.githubusercontent.com/u/2041385?v=4" width="100px;" alt="Phil Plückthun"/><br /><sub><b>Phil Plückthun</b></sub></a><br /><a href="#ideas-kitten" title="Ideas, Planning, & Feedback">🤔</a></td>
    <td align="center"><a href="https://github.com/kiraarghy"><img src="https://avatars2.githubusercontent.com/u/21056165?v=4" width="100px;" alt="Kara Stubbs"/><br /><sub><b>Kara Stubbs</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=kiraarghy" title="Code">💻</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=kiraarghy" title="Tests">⚠️</a> <a href="#example-kiraarghy" title="Examples">💡</a></td>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/oddlyfunctional"><img src="https://avatars1.githubusercontent.com/u/565635?v=4" width="100px;" alt="Marcos Felipe Pimenta Rodrigues"/><br /><sub><b>Marcos Felipe Pimenta Rodrigues</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=oddlyfunctional" title="Documentation">📖</a></td>
    <td align="center"><a href="https://github.com/gugahoa"><img src="https://avatars2.githubusercontent.com/u/1438470?v=4" width="100px;" alt="Gustavo Aguiar"/><br /><sub><b>Gustavo Aguiar</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=gugahoa" title="Code">💻</a> <a href="#example-gugahoa" title="Examples">💡</a></td>
    <td align="center"><a href="https://github.com/Schmavery"><img src="https://avatars1.githubusercontent.com/u/2154522?v=4" width="100px;" alt="Avery Morin"/><br /><sub><b>Avery Morin</b></sub></a><br /><a href="#ideas-Schmavery" title="Ideas, Planning, & Feedback">🤔</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=Schmavery" title="Code">💻</a> <a href="#example-Schmavery" title="Examples">💡</a></td>
    <td align="center"><a href="https://medium.com/@idkjs"><img src="https://avatars1.githubusercontent.com/u/2370391?v=4" width="100px;" alt="Alain Armand"/><br /><sub><b>Alain Armand</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=idkjs" title="Code">💻</a> <a href="#example-idkjs" title="Examples">💡</a></td>
  </tr>
  <tr>
    <td align="center"><a href="http://weser.io"><img src="https://avatars0.githubusercontent.com/u/10060928?v=4" width="100px;" alt="Robin Weser"/><br /><sub><b>Robin Weser</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=robinweser" title="Documentation">📖</a></td>
  </tr>
</table>

<!-- ALL-CONTRIBUTORS-LIST:END -->

## Maintenance Status

**Experimental:** This project is quite new. We're not sure what our ongoing maintenance plan for this project will be. Bug reports, feature requests and pull requests are welcome. If you like this project, let us know!

[maintenance-image]: https://img.shields.io/badge/maintenance-experimental-blueviolet.svg
