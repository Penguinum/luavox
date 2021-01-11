local lv = require "luavox"

describe("luavox library", function()
  it("does init", function()
    local version = lv.init(0, 44100, 2, { offline = true })
    assert.is.truthy(version > 0)
  end)

  it("does init without flags", function()
    local version = lv.init(0, 44100, 2)
    assert.is.truthy(version > 0)
  end)

  it("does load file", function()
    lv.open_slot(0)
    lv.load(0, "test.sunvox")
    lv.volume(0, 256)
    lv.play_from_beginning(0)
  end)

  it("does work", function()
    local ret_code, time_map = lv.get_time_map(0, 0, 100, { dest = "speed" })
    assert.is.equal(ret_code, 0)
  end)
end)
