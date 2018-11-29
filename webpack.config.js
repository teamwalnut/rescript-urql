const path = require("path");

const isProd = process.env.NODE_ENV === "production";

module.exports = {
  entry: path.join(__dirname, "example/app/Example.bs.js"),
  mode: isProd ? "production" : "development",
  output: {
    path: path.resolve(__dirname, "example/build"),
    publicPath: "/public/",
    filename: "Index.js"
  },
  devServer: {
    open: true,
    contentBase: path.resolve(__dirname, "example/public")
  },
  module: {
    rules: [
      {
        test: /\.svg$/,
        loader: "url-loader"
      }
    ]
  }
};
