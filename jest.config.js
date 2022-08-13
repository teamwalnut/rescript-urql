module.exports = {
  moduleFileExtensions: ["js", "mjs"],
  testMatch: ["**/__tests__/**/*_test.mjs", "**/__tests__/**/*_test.bs.js"],
  transform: {
    "^.+.m?js$": "babel-jest",
  },
  transformIgnorePatterns: ["node_modules/(?!(@glennsl/rescript-jest|bs-fetch|wonka|rescript)/)"],
};
