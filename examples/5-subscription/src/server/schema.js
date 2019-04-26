const { PubSub } = require("graphql-subscriptions");
const { HarryPotter } = require("fakergem");

const pubsub = new PubSub();

const store = {
  messages: [],
  numbers: []
};

const typeDefs = `
type Query {
  messages: [Message!]!
  numbers: [Int!]!
}
type Subscription {
  newMessage: Message!
  newNumber: Int!
}
type Message {
  id: ID!,
  message: String!,
}
`;

const resolvers = {
  Query: {
    messages: store.messages,
    numbers: store.numbers
  },
  Subscription: {
    newMessage: {
      subscribe: () => pubsub.asyncIterator("newMessage")
    },
    newNumber: {
      subscribe: () => pubsub.asyncIterator("newNumber")
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

setInterval(() =>
  pubsub.publish("newNumber", {
    newNumber: getRandomInt(1000)
  })
);
