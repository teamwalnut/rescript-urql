const { makeExecutableSchema } = require('graphql-tools');
require('isomorphic-fetch');
require('es6-promise').polyfill();

let dogs;
fetch('https://rawgit.com/FormidableLabs/dogs/master/dogs.json')
  .then(res => res.json())
  .then((res) => {
    dogs = res.map(dog => Object.assign({}, dog, { likes: 0 }))
  })
  .catch(err => console.log(err));

const typeDefs = `
  type Query {
    dogs: [Dog]
    dog(key: ID!): Dog
  }
  type Mutation {
    likeDog(key: ID!): Dog
    likeAllDogs: [Dog]
  }
  type Dog {
    key: String
    name: String
    breed: String
    color: String
    imageUrl: String
    description: String
    likes: Int
  }
`;

const resolvers = {
  Query: {
    dogs: (root, args, context) => {
      return dogs;
    },
    dog: (roots, args, context) => {
      return dogs.find(a => a.key === args.key);
    }
  },
  Mutation: {
    likeDog: (root, args, context) => {
      const dog = dogs.find(a => a.key === args.key);
      const idx = dogs.indexOf(dog);
      const liked = {
        ...dog,
        likes: dog.likes + 1
      };
      dogs.splice(idx, 1, liked);
      return liked;
    },
    likeAllDogs: (root, args, context) => {
      dogs.forEach(dog => {
        dog.likes = dog.likes + 1;
      });
      return dogs;
    }
  }
};

module.exports = {
  schema: makeExecutableSchema({
    typeDefs,
    resolvers,
  }),
  context: (headers, secrets) => {
    return {
      headers,
      secrets,
    };
  },
};