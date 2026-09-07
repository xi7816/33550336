import { describe, it, expect } from 'vitest'
import { Board } from '../Board'

describe('Board', () => {
  describe('构造与尺寸校验', () => {
    it('合法尺寸创建成功', () => {
      expect(new Board(5).size).toBe(5)
      expect(new Board(9).size).toBe(9)
      expect(new Board(15).size).toBe(15)
      expect(new Board(19).size).toBe(19)
    })

    it('尺寸 < 5 抛错', () => {
      expect(() => new Board(3)).toThrow()
      expect(() => new Board(4)).toThrow()
    })

    it('尺寸 > 19 抛错', () => {
      expect(() => new Board(20)).toThrow()
      expect(() => new Board(21)).toThrow()
    })

    it('偶数尺寸抛错', () => {
      expect(() => new Board(10)).toThrow()
      expect(() => new Board(14)).toThrow()
    })
  })

  describe('位置校验', () => {
    const board = new Board(15)

    it('边界坐标合法', () => {
      expect(board.isValidPosition({ row: 1, col: 1 })).toBe(true)
      expect(board.isValidPosition({ row: 15, col: 15 })).toBe(true)
    })

    it('越界坐标非法', () => {
      expect(board.isValidPosition({ row: 0, col: 1 })).toBe(false)
      expect(board.isValidPosition({ row: 1, col: 0 })).toBe(false)
      expect(board.isValidPosition({ row: 16, col: 1 })).toBe(false)
      expect(board.isValidPosition({ row: 1, col: 16 })).toBe(false)
    })
  })

  describe('棋子操作', () => {
    it('placeStone 后 getCell 返回对应状态', () => {
      const board = new Board(15)
      board.placeStone({ row: 8, col: 8 }, 'Black')
      expect(board.getCell({ row: 8, col: 8 })).toBe('Black')
      board.placeStone({ row: 1, col: 1 }, 'White')
      expect(board.getCell({ row: 1, col: 1 })).toBe('White')
    })

    it('clearStone 后位置变空', () => {
      const board = new Board(15)
      board.placeStone({ row: 8, col: 8 }, 'Black')
      board.clearStone({ row: 8, col: 8 })
      expect(board.isEmpty({ row: 8, col: 8 })).toBe(true)
    })

    it('isEmpty 初始全空', () => {
      const board = new Board(9)
      for (let r = 1; r <= 9; r++) {
        for (let c = 1; c <= 9; c++) {
          expect(board.isEmpty({ row: r, col: c })).toBe(true)
        }
      }
    })
  })

  describe('isFull 与 reset', () => {
    it('空棋盘 isFull 为 false', () => {
      expect(new Board(5).isFull()).toBe(false)
    })

    it('填满后 isFull 为 true', () => {
      const board = new Board(5)
      for (let r = 1; r <= 5; r++) {
        for (let c = 1; c <= 5; c++) {
          board.placeStone({ row: r, col: c }, 'Black')
        }
      }
      expect(board.isFull()).toBe(true)
    })

    it('reset 后全空', () => {
      const board = new Board(5)
      board.placeStone({ row: 1, col: 1 }, 'Black')
      board.reset()
      expect(board.isEmpty({ row: 1, col: 1 })).toBe(true)
      expect(board.isFull()).toBe(false)
    })
  })
})