module.exports = require("babel-jest").default.createTransformer({
  presets: [
    [
      "@babel/preset-react",
      {
        runtime: "automatic",
      },
    ],
  ],
  plugins: [
    [
      "@babel/plugin-transform-modules-commonjs",
      {
        importInterop: (specifier) => {
          if (specifier == "jest-fail-on-console") {
            return "babel";
          } else {
            return "none";
          }
        },
      },
    ],
  ],
});
