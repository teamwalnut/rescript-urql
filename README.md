# reason-urql

[![npm](https://img.shields.io/npm/v/reason-urql.svg)](https://www.npmjs.com/package/reason-urql)
[![All Contributors](https://img.shields.io/badge/all_contributors-16-orange.svg)](#contributors)
[![Build Status](https://travis-ci.com/FormidableLabs/reason-urql.svg?branch=master)](https://travis-ci.com/FormidableLabs/reason-urql)
[![Maintenance Status][maintenance-image]](#maintenance-status)
[![Spectrum](https://withspectrum.github.io/badge/badge.svg)](https://spectrum.chat/urql)

Reason bindings for Formidable's Universal React Query Library, [`urql`](https://github.com/FormidableLabs/urql).

## ✨Features

- ⚛️ A fully featured GraphQL client for ReasonReact.
- ✅ Compile time type and schema validation.
- ⚙️ Customizable behavior via `exchanges`.
- 🎣 Support for `useQuery`, `useMutation`, `useDynamicMutation`, `useSubscription`, and `useClient` hooks!
- ⚡ Support for server-side rendering with Next.js.

`reason-urql` is a GraphQL client for ReasonReact, allowing you to hook up your ReasonReact components to queries, mutations, and subscriptions. It provides bindings to `urql` that allow you to use the API in Reason, with the benefits of a sound type system, blazing fast compilation, and opportunities for guided customization.

## 📋 Documentation

- [Getting Started](/docs/getting-started.md)
- [API](/docs)

## 💾 Installation

#### 1. Install `reason-urql`.

```sh
yarn add reason-urql
```

#### 2. Add `graphql_ppx_re`.

To get the most out of compile time type checks for your GraphQL queries, mutations, and subscriptions, we recommend using [`graphql_ppx_re`](https://github.com/reasonml-community/graphql_ppx). `useDynamicMutation` in particular takes advantage of some of its internals for an excellent experience writing type safe code to access your GraphQL responses.

```sh
yarn add @baransu/graphql_ppx_re --dev
```

#### 3. Update `bsconfig.json`.

Add `reason-urql` to your `bs-dependencies` and `graphql_ppx_re` to your `ppx_flags` in `bsconfig.json`.

```json
{
  "bs-dependencies": ["reason-urql"],
  "ppx-flags": ["@baransu/graphql_ppx_re/ppx"]
}
```

#### 4. Send an introspection query to your API.

Finally, you'll need to send an introspection query to your GraphQl API, using a tool like [`graphql-cli`](https://github.com/Urigo/graphql-cli/). You should generate a file called `graphql_schema.json` at the root of your project that `graphql_ppx_re` can use to type check your queries. **You should check this file into version control** and keep it updated as your API changes.

For additional instructions, head [here](https://github.com/reasonml-community/graphql_ppx#usage).

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
    <td align="center"><a href="http://parkerziegler.com/"><img src="https://avatars0.githubusercontent.com/u/19421190?v=4" width="100px;" alt=""/><br /><sub><b>Parker Ziegler</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=parkerziegler" title="Code">💻</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=parkerziegler" title="Documentation">📖</a> <a href="https://github.com/FormidableLabs/reason-urql/pulls?q=is%3Apr+reviewed-by%3Aparkerziegler" title="Reviewed Pull Requests">👀</a> <a href="#ideas-parkerziegler" title="Ideas, Planning, & Feedback">🤔</a></td>
    <td align="center"><a href="https://khoanguyen.me"><img src="https://avatars2.githubusercontent.com/u/3049054?v=4" width="100px;" alt=""/><br /><sub><b>Khoa Nguyen</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=thangngoc89" title="Code">💻</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=thangngoc89" title="Documentation">📖</a></td>
    <td align="center"><a href="https://twitter.com/_philpl"><img src="https://avatars0.githubusercontent.com/u/2041385?v=4" width="100px;" alt=""/><br /><sub><b>Phil Plückthun</b></sub></a><br /><a href="#ideas-kitten" title="Ideas, Planning, & Feedback">🤔</a></td>
    <td align="center"><a href="https://github.com/kiraarghy"><img src="https://avatars2.githubusercontent.com/u/21056165?v=4" width="100px;" alt=""/><br /><sub><b>Kara Stubbs</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=kiraarghy" title="Code">💻</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=kiraarghy" title="Tests">⚠️</a> <a href="#example-kiraarghy" title="Examples">💡</a></td>
    <td align="center"><a href="https://github.com/oddlyfunctional"><img src="https://avatars1.githubusercontent.com/u/565635?v=4" width="100px;" alt=""/><br /><sub><b>Marcos Felipe Pimenta Rodrigues</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=oddlyfunctional" title="Documentation">📖</a></td>
    <td align="center"><a href="https://github.com/gugahoa"><img src="https://avatars2.githubusercontent.com/u/1438470?v=4" width="100px;" alt=""/><br /><sub><b>Gustavo Aguiar</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=gugahoa" title="Code">💻</a> <a href="#example-gugahoa" title="Examples">💡</a></td>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/Schmavery"><img src="https://avatars1.githubusercontent.com/u/2154522?v=4" width="100px;" alt=""/><br /><sub><b>Avery Morin</b></sub></a><br /><a href="#ideas-Schmavery" title="Ideas, Planning, & Feedback">🤔</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=Schmavery" title="Code">💻</a> <a href="#example-Schmavery" title="Examples">💡</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=Schmavery" title="Documentation">📖</a></td>
    <td align="center"><a href="https://medium.com/@idkjs"><img src="https://avatars1.githubusercontent.com/u/2370391?v=4" width="100px;" alt=""/><br /><sub><b>Alain Armand</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=idkjs" title="Code">💻</a> <a href="#example-idkjs" title="Examples">💡</a></td>
    <td align="center"><a href="http://weser.io"><img src="https://avatars0.githubusercontent.com/u/10060928?v=4" width="100px;" alt=""/><br /><sub><b>Robin Weser</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=robinweser" title="Documentation">📖</a></td>
    <td align="center"><a href="https://ce.ms"><img src="https://avatars3.githubusercontent.com/u/959142?v=4" width="100px;" alt=""/><br /><sub><b>Cem Turan</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=cem2ran" title="Documentation">📖</a></td>
    <td align="center"><a href="https://www.huy.dev/"><img src="https://avatars1.githubusercontent.com/u/7352279?v=4" width="100px;" alt=""/><br /><sub><b>Huy Nguyen</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=huy-nguyen" title="Documentation">📖</a></td>
    <td align="center"><a href="http://www.riseos.com"><img src="https://avatars2.githubusercontent.com/u/35296?v=4" width="100px;" alt=""/><br /><sub><b>Sean Grove</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=sgrove" title="Code">💻</a> <a href="#example-sgrove" title="Examples">💡</a> <a href="#ideas-sgrove" title="Ideas, Planning, & Feedback">🤔</a> <a href="https://github.com/FormidableLabs/reason-urql/commits?author=sgrove" title="Documentation">📖</a></td>
  </tr>
  <tr>
    <td align="center"><a href="https://twitter.com/_cichocinski"><img src="https://avatars2.githubusercontent.com/u/9558691?v=4" width="100px;" alt=""/><br /><sub><b>Tomasz Cichocinski</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=baransu" title="Code">💻</a> <a href="https://github.com/FormidableLabs/reason-urql/issues?q=author%3Abaransu" title="Bug reports">🐛</a></td>
    <td align="center"><a href="https://www.jovidecroock.com/"><img src="https://avatars3.githubusercontent.com/u/17125876?v=4" width="100px;" alt=""/><br /><sub><b>Jovi De Croock</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=JoviDeCroock" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/tatchi"><img src="https://avatars2.githubusercontent.com/u/5595092?v=4" width="100px;" alt=""/><br /><sub><b>Corentin Leruth</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=tatchi" title="Documentation">📖</a></td>
    <td align="center"><a href="https://github.com/jeddeloh"><img src="https://avatars3.githubusercontent.com/u/1131723?v=4" width="100px;" alt=""/><br /><sub><b>Joel Jeddeloh</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=jeddeloh" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/hulufei"><img src="https://avatars0.githubusercontent.com/u/261677?v=4" width="100px;" alt=""/><br /><sub><b>hui.liu</b></sub></a><br /><a href="https://github.com/FormidableLabs/reason-urql/commits?author=hulufei" title="Documentation">📖</a></td>
  </tr>
</table>

<!-- markdownlint-enable -->
<!-- prettier-ignore-end -->
<!-- ALL-CONTRIBUTORS-LIST:END -->

## Maintenance Status

**Experimental:** This project is quite new. We're not sure what our ongoing maintenance plan for this project will be. Bug reports, feature requests and pull requests are welcome. If you like this project, let us know!

[maintenance-image]: https://img.shields.io/badge/maintenance-experimental-blueviolet.svg
