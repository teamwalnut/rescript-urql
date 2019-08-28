## Contributing to `reason-urql`

Thank you so much for contributing to `reason-urql`! We're excited you want to help make this project better with us.

`reason-urql` follows the [all contributors spec](https://allcontributors.org/). We believe firmly that the whole community is vital to our success, not just those who contribute code. The best way to get involved is to start by familiarizing yourself with the [`urql`](https://github.com/FormidableLabs/urql/) API and getting familiar with the basics of [Reason](https://reasonml.github.io/) and [BuckleScript](https://bucklescript.github.io/).

### How do I contribute?

We're always open to new issues and pull requests. For issues, we'll do our best to help spec out new features or bug fixes with you. We'll also tag issues to make them easier to identify as you get familiar with things. For pull requests, we're interested in all of it – new features, bug fixes, tests, docs, even the smallest typo!

If you do open a pull request for a new feature or bug, please consider adding tests in the `__tests__` directory. If your pull request fixes an issue, please list the issue number in the title or refer to it in the body of the description.

### How do I set up the project?

This is pretty standard. Simply clone the repo locally:

```sh
git clone https://github.com/FormidableLabs/reason-urql.git
```

and install the dependencies:

```sh
cd reason-urql
yarn
```

We _really_ recommend having an editor plugin to run `refmt`, provide inline type annotations, and provide syntax highlighting. [`reason-language-server`](https://github.com/jaredly/reason-language-server) by Jared Forsyth is really excellent for providing all of these features out of the box!

#### Compiling the Source

To compile the source and watch for changes:

```sh
yarn start
```

To build the source:

```sh
yarn build
```

To clean out any build artifacts:

```sh
yarn clean
```

#### Running the Demo

To run the demo, simply invoke the `start:demo` script:

```sh
yarn start:demo
```

It's often best to compile the source in watch mode (`yarn start`) and run the `start:demo` script _in a separate terminal_ so you can get the full live reloading experience.

#### Running Tests

To run all test suites:

```sh
yarn test
```

We use [`bs-jest`](https://github.com/glennsl/bs-jest) to compile and run our tests. You can pass standard [Jest CLI options](https://jestjs.io/docs/en/cli) to this command if you want to do something particular, like run in watch mode:

```sh
yarn test --watch
```

or target a specific test:

```sh
yarn test -t 'should instantiate a client instance'
```

To get coverage statistics:

```sh
yarn coverage
```

#### `refmt`

You can install `refmt` globally by following the installation instructions for [`reason-cli`](https://github.com/reasonml/reason-cli). This will put a lot of nice helpers in your path. You can run `refmt` over the source using the CLI:

```sh
refmt --in-place src/*.re
```

We recommend letting the editor take care of this for you by installing `reason-language-server`. We don't currently enforce a specific `refmt` version, but please always attempt to use the latest version for your OS.

### Publishing

When it comes time to publish a new version of `reason-urql`, we follow a pretty standard workflow using [semantic versioning](https://semver.org/). **Make sure you have, and are on, latest `master` before publishing.**

```sh
yarn version --<major | minor | patch>
yarn publish
git push && git push --tags
```

#### Drafting a Release

Once you have succesfully published to `npm`, please consider [drafting a release](https://github.com/FormidableLabs/reason-urql/releases). We like to follow the format outlined by [Keep a Changleog](https://keepachangelog.com/en/1.0.0/). In addition to drafting a release, please also update the CHANGELOG after publishing with the same copy you used to draft the release.
