"use strict";

const { makeExecutableSchema } = require("graphql-tools");

const typeDefs = `
  type Query {
    dogs: [Dog]!
    dog(key: ID!): Dog!
  }
  type Mutation {
    likeDog(key: ID!): Dog!
    likeAllDogs: [Dog]!
  }
  type Dog {
    key: String!
    name: String!
    breed: String!
    color: String!
    imageUrl: String!
    description: String!
    likes: Int!
  }
`;

const resolvers = dogs => ({
  Query: {
    dogs: (root, args) => dogs,
    dog: (root, args) => dogs.find(a => a.key === args.key)
  },
  Mutation: {
    likeDog: (root, args) => {
      const dog = dogs.find(a => a.key === args.key);
      const idx = dogs.indexOf(dog);
      const liked = {
        ...dog,
        likes: dog.likes + 1
      };
      dogs.splice(idx, 1, liked);
      return liked;
    },
    likeAllDogs: (root, args) => {
      dogs.forEach(dog => {
        dog.likes += 1;
      });
      return dogs;
    }
  }
});

module.exports = {
  schema: dogs =>
    makeExecutableSchema({
      typeDefs,
      resolvers: resolvers(dogs)
    })
};
