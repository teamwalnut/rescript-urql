## Contributing to `rescript-urql`

Thank you so much for contributing to `rescript-urql`! We're excited you want to help make this project better with us.

`rescript-urql` follows the [all contributors spec](https://allcontributors.org/). We believe firmly that the whole community is vital to our success, not just those who contribute code. The best way to get involved is to start by familiarizing yourself with the [`urql`](https://github.com/FormidableLabs/urql/) API and getting familiar with the basics of [ReScript](https://rescript-lang.org/).

### How do I contribute?

We're always open to new issues and pull requests. For issues, we'll do our best to help spec out new features or bug fixes with you. We'll also tag issues to make them easier to identify as you get familiar with things. For pull requests, we're interested in all of it – new features, bug fixes, tests, docs, even the smallest typo!

If you do open a pull request for a new feature or bug, please consider adding tests in the `__tests__` directory. If your pull request fixes an issue, please list the issue number in the title or refer to it in the body of the description.

### How do I set up the project?

This is pretty standard. Clone the repo locally:

```sh
git clone https://github.com/FormidableLabs/rescript-urql.git
```

and install the dependencies:

```sh
cd rescript-urql
yarn
```

We _really_ recommend having an editor plugin to run the ReScript language server. For VSCode users, [`rescript-vscode`](https://github.com/rescript-lang/rescript-vscode) is the best option.

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

#### Formatting

Formatting will be handled automatically for you by [`rescript-vscode`](https://github.com/rescript-lang/rescript-vscode). If you're using a different editor than VSCode, check out [the official ReScript plugins](https://rescript-lang.org/docs/manual/latest/editor-plugins) for your editor of choice.

### Publishing

Prior to publishing to `npm`, please consider drafting a release. We like to follow the format outlined by [Keep a Changelog](https://keepachangelog.com/en/1.0.0/). To draft a release, add a new entry to CHANGELOG.md. Commit this change in a separate commit following the format:

```sh
git commit -m "Prepare vX.X.X release."
git push origin main
```

#### Publishing to NPM

Once you've added the CHANGELOG update and pushed the commit, you're ready to publish. We follow a pretty standard workflow using [semantic versioning](https://semver.org/). **Make sure you are on latest `main` before publishing.**

```sh
git pull origin main

# Assuming you have latest main...
yarn version --<major | minor | patch>
yarn publish
git push && git push --tags
```

Once the release is published, make sure you copy the CHANGELOG update to [the formal Releases page](https://github.com/FormidableLabs/rescript-urql/releases).
