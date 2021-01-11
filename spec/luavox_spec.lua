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

  describe("get_time_map", function()
    it("works", function()
      local ret_code, time_map = lv.get_time_map(0, 0, 100, { dest = "speed" })
      assert.is.equal(ret_code, 0)
      assert.is.truthy(type(time_map) == "table")
      assert.is.truthy(type(time_map[1]) == "number")
    end)

    it("does require flags", function()
      local ok, err = pcall(function()
        local ret_code, time_map = lv.get_time_map(0, 0, 100)
      end)
      assert.is.truthy(
        not ok and type(err) == "string" and err:match("table expected")
      )
    end)

    it("produces error with wrong flags", function()
      local ok, err = pcall(function()
        local ret_code, time_map = lv.get_time_map(0, 0, 100, { dest = "shpeed" })
      end)
      assert.is.truthy(
        not ok and type(err) == "string" and err:match("Bad parameter value")
      )
    end)
  end)
end)
