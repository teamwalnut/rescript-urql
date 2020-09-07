const path = require("path");

module.exports = {
  entry: path.join(__dirname, "src/app/index.bs.js"),
  mode: "development",
  output: {
    path: path.resolve(__dirname, "build"),
    publicPath: "/public/",
    filename: "index.js",
  },
  devServer: {
    contentBase: path.resolve(__dirname, "public"),
    port: 3000,
    hot: true,
  },
  resolve: {
    alias: {
      react: path.resolve("./node_modules/react"),
      "react-dom": path.resolve("./node_modules/react-dom"),
      urql: path.resolve("../../node_modules/urql"),
    },
  },
};
