local lv = require "luavox"

describe("luavox library", function()
  it("does init", function()
    local version = lv.init(0, 44100, 2, { offline = true })
    assert.is.truthy(version > 0)
  end)
end)
