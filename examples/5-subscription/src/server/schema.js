const { PubSub } = require("graphql-subscriptions");
const { HarryPotter } = require("fakergem");

const pubsub = new PubSub();

const store = {
  messages: [],
  numbers: [],
  floats: []
};

const typeDefs = `
type Query {
  messages: [Message!]!
  numbers: [Int!]!
  floats: [Float!]!
}
type Subscription {
  newMessage: Message!
  newNumber: Int!
  newFloat: Float!
}
type Message {
  id: ID!,
  message: String!,
}
`;

const resolvers = {
  Query: {
    messages: store.messages,
    numbers: store.numbers,
    floats: store.floats
  },
  Subscription: {
    newMessage: {
      subscribe: () => pubsub.asyncIterator("newMessage")
    },
    newNumber: {
      subscribe: () => pubsub.asyncIterator("newNumber")
    },
    newFloat: {
      subscribe: () => pubsub.asyncIterator("newFloat")
    }
  }
};

module.exports = {
  typeDefs,
  resolvers,
  context: (headers, secrets) => {
    return {
      headers,
      secrets
    };
  }
};

let id = 0;

setInterval(() => {
  const newMessage = {
    id: ++id,
    message: HarryPotter.quote()
  };

  pubsub.publish("newMessage", {
    newMessage
  });
}, 5000);

function getRandomInt(max) {
  return Math.floor(Math.random() * Math.floor(max));
}

setInterval(() => {
  pubsub.publish("newNumber", {
    newNumber: getRandomInt(1000)
  });
}, 2000);

setInterval(() => {
  pubsub.publish("newFloat", {
    newFloat: Math.random() * 1000
  });
}, 500);
