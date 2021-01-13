# reason-urql

[![npm](https://img.shields.io/npm/v/reason-urql.svg)](https://www.npmjs.com/package/reason-urql)
[![All Contributors](https://img.shields.io/badge/all_contributors-20-orange.svg)](#contributors)
[![Build Status](https://github.com/FormidableLabs/reason-urql/workflows/reason-urql%20CI/badge.svg)](https://github.com/FormidableLabs/reason-urql/actions?query=workflow%3A%22reason-urql+CI%22)
[![Maintenance Status][maintenance-image]](#maintenance-status)

Reason bindings for Formidable's Universal React Query Library, [`urql`](https://github.com/FormidableLabs/urql).

## ✨Features

- ⚛️ A fully featured GraphQL client for ReasonReact.
- ✅ Compile time type and schema validation.
- ⚙️ Customizable behavior via `exchanges`.
- 🎣 Support for `useQuery`, `useMutation`, `useSubscription`, and `useClient` hooks!
- ⚡ Support for server-side rendering with Next.js.

`reason-urql` is a GraphQL client for ReasonReact, allowing you to hook up your ReasonReact components to queries, mutations, and subscriptions. It provides bindings to `urql` that allow you to use the API in Reason, with the benefits of a sound type system, blazing fast compilation, and opportunities for guided customization.

## 📋 Documentation

- [Getting Started](/docs/getting-started.md)
- [Client and Provider](/docs/client-and-provider.md)
- [Hooks](/docs/hooks.md)
- [Exchanges](/docs/exchanges.md)
- [Errors](/docs/error.md)
- [Advanced](/docs/advanced.md)

## 💾 Installation

### 1. Install `reason-urql` alongside its `peerDependencies` and `devDependencies`.

```sh
yarn add reason-urql urql graphql
yarn add gentype --dev
```

We try to keep our bindings as close to latest `urql` as possible. However, `urql` tends to make releases a bit ahead of `reason-urql`. To get a compatible version, we recommend always staying strictly within this project's `peerDependency` range for `urql`.

The `gentype` `devDependency` is a requirement introduced by `urql`'s use of `wonka`. `wonka`'s source uses `@genType` declarations, so when the BuckleScript / ReScript compiler attempts to compile `wonka` in your project, it'll need a local copy of `gentype` to use.

#### 1a. **Important note for users of `bs-platform>=8.0.0`**.

If using `bs-platform>=8.0.0` you'll need to use [`yarn resolutions`](https://classic.yarnpkg.com/en/docs/selective-version-resolutions/) to specify a specific version of `wonka` to resolve. `urql` has an explicit dependency on latest `wonka` `v4`, which is incompatible with `bs-platform>=8.0.0`. See [this issue](https://github.com/kitten/wonka/issues/85) for more details.

In your `package.json`, add the following:

```json
"resolutions": {
  "wonka": "5.0.0-rc.1"
}
```

If you're using `npm`, you may need to stay on `bs-platform@7.3.2` until `urql` takes a dependency on `wonka>=5.0.0`.

### 2. Add `@reasonml-community/graphql-ppx`.

To get the most out of compile time type checks for your GraphQL queries, mutations, and subscriptions, we use [`@reasonml-community/graphql-ppx`](https://github.com/reasonml-community/graphql-ppx). Add this to your project's `devDependencies`.

```sh
yarn add @reasonml-community/graphql-ppx --dev
```

### 3. Update `bsconfig.json`.

Add `reason-urql`, `wonka`, and `@reasonml-community/graphql-ppx` to your `bs-dependencies` and `@reasonml-community/graphql-ppx/ppx` to your `ppx_flags` in `bsconfig.json`.

```json
{
  "bs-dependencies": [
    "reason-urql",
    "wonka",
    "@reasonml-community/graphql-ppx"
  ],
  "ppx-flags": ["@reasonml-community/graphql-ppx/ppx"]
}
```

### 4. Send an introspection query to your API.

Finally, you'll need to send an introspection query to your GraphQl API, using a tool like [`graphql-cli`](https://github.com/Urigo/graphql-cli/). You should generate a file called `graphql_schema.json` at the root of your project that `graphql-ppx` can use to type check your queries. **You should check this file into version control** and keep it updated as your API changes.

For additional help, head [here](https://github.com/reasonml-community/graphql-ppx#schema).

```sh
npx get-graphql-schema ENDPOINT_URL -j > graphql_schema.json
```

Simply re-run this script at anytime to regenerate the `graphql_schema.json` file according to your latest backend schema.

## 💻 Example Projects

`reason-urql` has a nice set of examples showing how to use the hooks and client APIs to get the most out of GraphQL and Reason in your app – check them out in the `/examples` folder. To run any of the examples, follow these steps.

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

The example will start up at `http://localhost:3000`. Edit the example freely to watch changes take effect.

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

Please help out by [opening an issue](https://github.com/FormidableLabs/reason-urql/issues) or [filing a PR](https://github.com/FormidableLabs/reason-urql/pulls).

## Contributors

This project follows the [all contributors spec](https://github.com/kentcdodds/all-contributors). Thanks to these wonderful folks for contributing ([Emoji Key](https://github.com/kentcdodds/all-contributors#emoji-key)):

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center"><a href="http://parkerziegler.com/"><img src="https://avatars0.githubusercontent.com/u/19421190?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Parker Ziegler</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=parkerziegler" title="Code">💻</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=parkerziegler" title="Documentation">📖</a> <a href="https://github.com/FormidableLabs/reason-urql/pulls?q=is%3Apr+reviewed-by%3Aparkerziegler" title="Reviewed Pull Requests">👀</a> <a href="#ideas-parkerziegler" title="Ideas, Planning, & Feedback">🤔</a></td>
    <td align="center"><a href="https://khoanguyen.me"><img src="https://avatars2.githubusercontent.com/u/3049054?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Khoa Nguyen</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=thangngoc89" title="Code">💻</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=thangngoc89" title="Documentation">📖</a></td>
    <td align="center"><a href="https://twitter.com/_philpl"><img src="https://avatars0.githubusercontent.com/u/2041385?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Phil Plückthun</b></sub></a><br /><a href="#ideas-kitten" title="Ideas, Planning, & Feedback">🤔</a></td>
    <td align="center"><a href="https://github.com/kiraarghy"><img src="https://avatars2.githubusercontent.com/u/21056165?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Kara Stubbs</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=kiraarghy" title="Code">💻</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=kiraarghy" title="Tests">⚠️</a> <a href="#example-kiraarghy" title="Examples">💡</a></td>
    <td align="center"><a href="https://github.com/oddlyfunctional"><img src="https://avatars1.githubusercontent.com/u/565635?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Marcos Felipe Pimenta Rodrigues</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=oddlyfunctional" title="Documentation">📖</a></td>
    <td align="center"><a href="https://github.com/gugahoa"><img src="https://avatars2.githubusercontent.com/u/1438470?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Gustavo Aguiar</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=gugahoa" title="Code">💻</a> <a href="#example-gugahoa" title="Examples">💡</a></td>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/Schmavery"><img src="https://avatars1.githubusercontent.com/u/2154522?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Avery Morin</b></sub></a><br /><a href="#ideas-Schmavery" title="Ideas, Planning, & Feedback">🤔</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=Schmavery" title="Code">💻</a> <a href="#example-Schmavery" title="Examples">💡</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=Schmavery" title="Documentation">📖</a></td>
    <td align="center"><a href="https://medium.com/@idkjs"><img src="https://avatars1.githubusercontent.com/u/2370391?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Alain Armand</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=idkjs" title="Code">💻</a> <a href="#example-idkjs" title="Examples">💡</a></td>
    <td align="center"><a href="http://weser.io"><img src="https://avatars0.githubusercontent.com/u/10060928?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Robin Weser</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=robinweser" title="Documentation">📖</a></td>
    <td align="center"><a href="https://ce.ms"><img src="https://avatars3.githubusercontent.com/u/959142?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Cem Turan</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=cem2ran" title="Documentation">📖</a></td>
    <td align="center"><a href="https://www.huy.dev/"><img src="https://avatars1.githubusercontent.com/u/7352279?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Huy Nguyen</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=huy-nguyen" title="Documentation">📖</a></td>
    <td align="center"><a href="http://www.riseos.com"><img src="https://avatars2.githubusercontent.com/u/35296?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Sean Grove</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=sgrove" title="Code">💻</a> <a href="#example-sgrove" title="Examples">💡</a> <a href="#ideas-sgrove" title="Ideas, Planning, & Feedback">🤔</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=sgrove" title="Documentation">📖</a></td>
  </tr>
  <tr>
    <td align="center"><a href="https://twitter.com/_cichocinski"><img src="https://avatars2.githubusercontent.com/u/9558691?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Tomasz Cichocinski</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=baransu" title="Code">💻</a> <a href="https://github.com/FormidableLabs/reason-urql/issues?q=author%3Abaransu" title="Bug reports">🐛</a></td>
    <td align="center"><a href="https://www.jovidecroock.com/"><img src="https://avatars3.githubusercontent.com/u/17125876?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Jovi De Croock</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=JoviDeCroock" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/tatchi"><img src="https://avatars2.githubusercontent.com/u/5595092?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Corentin Leruth</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=tatchi" title="Documentation">📖</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=tatchi" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/jeddeloh"><img src="https://avatars3.githubusercontent.com/u/1131723?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Joel Jeddeloh</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=jeddeloh" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/hulufei"><img src="https://avatars0.githubusercontent.com/u/261677?v=4?s=100" width="100px;" alt=""/><br /><sub><b>hui.liu</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=hulufei" title="Documentation">📖</a></td>
    <td align="center"><a href="https://github.com/gaku-sei"><img src="https://avatars0.githubusercontent.com/u/3982371?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Kévin Combriat</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=gaku-sei" title="Code">💻</a> <a href="https://github.com/FormidableLabs/reason-urql/issues?q=author%3Agaku-sei" title="Bug reports">🐛</a> <a href="#ideas-gaku-sei" title="Ideas, Planning, & Feedback">🤔</a></td>
  </tr>
  <tr>
    <td align="center"><a href="http://amiralies.github.io"><img src="https://avatars3.githubusercontent.com/u/13261088?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Amirali Esmaeili</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=amiralies" title="Code">💻</a> <a href="#example-amiralies" title="Examples">💡</a></td>
    <td align="center"><a href="https://www.alexandervarwijk.com"><img src="https://avatars1.githubusercontent.com/u/327697?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Alexander Varwijk</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=Kingdutch" title="Code">💻</a></td>
  </tr>
</table>

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->

## Maintenance Status

**Experimental:** This project is quite new. We're not sure what our ongoing maintenance plan for this project will be. Bug reports, feature requests and pull requests are welcome. If you like this project, let us know!

[maintenance-image]: https://img.shields.io/badge/maintenance-experimental-blueviolet.svg
