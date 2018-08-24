"use strict";

const graphqlHttp = require("express-graphql");
const express = require("express");
const app = express();
const cors = require("cors");
const fetch = require("isomorphic-fetch");
require("es6-promise").polyfill();

const start = async () => {
  // make the req to RawGit for Formidable dogs
  const res = await fetch(
    "https://rawgit.com/FormidableLabs/dogs/master/dogs.json"
  );

  const data = await res.json();
  const dogs = data.map(dog => ({
    ...dog,
    likes: 0
  }));

  app.use(cors());

  const { schema } = require("./schema");

  const PORT = 3001;

  const initializedGraphQLMiddleware = graphqlHttp({
    // GraphQL’s data schema
    schema: schema(dogs),
    // Pretty Print the JSON response
    pretty: true,
    // Enable GraphiQL dev tool
    graphiql: true
  });

  app.use(initializedGraphQLMiddleware);

  app.listen(PORT, () => {
    // eslint-disable-next-line no-console
    console.log(`Server running at http://localhost:${PORT}`);
  });
};

start();
