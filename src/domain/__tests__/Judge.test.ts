import { describe, it, expect } from 'vitest'
import { Board } from '../Board'
import { checkWin, checkDraw } from '../Judge'

function placeLine(board: Board, player: 'Black' | 'White', row: number, col: number, dr: number, dc: number, count: number): void {
  for (let i = 0; i < count; i++) {
    board.placeStone({ row: row + dr * i, col: col + dc * i }, player)
  }
}

describe('Judge', () => {
  describe('checkWin 四方向', () => {
    it('横向五连', () => {
      const board = new Board(15)
      placeLine(board, 'Black', 7, 7, 0, 1, 5)
      const result = checkWin(board, { row: 7, col: 9 }, 'Black')
      expect(result).not.toBeNull()
      expect(result!.direction).toBe('Horizontal')
      expect(result!.length).toBe(5)
      expect(result!.start).toEqual({ row: 7, col: 7 })
    })

    it('纵向五连', () => {
      const board = new Board(15)
      placeLine(board, 'White', 5, 5, 1, 0, 5)
      const result = checkWin(board, { row: 7, col: 5 }, 'White')
      expect(result).not.toBeNull()
      expect(result!.direction).toBe('Vertical')
      expect(result!.length).toBe(5)
    })

    it('左斜五连', () => {
      const board = new Board(15)
      placeLine(board, 'Black', 5, 5, 1, 1, 5)
      const result = checkWin(board, { row: 7, col: 7 }, 'Black')
      expect(result).not.toBeNull()
      expect(result!.direction).toBe('LeftDiagonal')
      expect(result!.length).toBe(5)
    })

    it('右斜五连', () => {
      const board = new Board(15)
      placeLine(board, 'Black', 5, 9, 1, -1, 5)
      const result = checkWin(board, { row: 7, col: 7 }, 'Black')
      expect(result).not.toBeNull()
      expect(result!.direction).toBe('RightDiagonal')
      expect(result!.length).toBe(5)
    })
  })

  describe('checkWin 长连与边界', () => {
    it('六连（长连）判定获胜', () => {
      const board = new Board(15)
      placeLine(board, 'Black', 7, 5, 0, 1, 6)
      const result = checkWin(board, { row: 7, col: 8 }, 'Black')
      expect(result).not.toBeNull()
      expect(result!.length).toBe(6)
    })

    it('四连不获胜', () => {
      const board = new Board(15)
      placeLine(board, 'Black', 7, 7, 0, 1, 4)
      const result = checkWin(board, { row: 7, col: 8 }, 'Black')
      expect(result).toBeNull()
    })

    it('无连珠返回 null', () => {
      const board = new Board(15)
      board.placeStone({ row: 7, col: 7 }, 'Black')
      const result = checkWin(board, { row: 7, col: 7 }, 'Black')
      expect(result).toBeNull()
    })

    it('边缘连珠', () => {
      const board = new Board(15)
      placeLine(board, 'Black', 1, 1, 0, 1, 5)
      const result = checkWin(board, { row: 1, col: 3 }, 'Black')
      expect(result).not.toBeNull()
      expect(result!.start).toEqual({ row: 1, col: 1 })
    })
  })

  describe('checkDraw', () => {
    it('空棋盘返回 false', () => {
      expect(checkDraw(new Board(15))).toBe(false)
    })

    it('全满返回 true', () => {
      const board = new Board(5)
      for (let r = 1; r <= 5; r++) {
        for (let c = 1; c <= 5; c++) {
          board.placeStone({ row: r, col: c }, 'Black')
        }
      }
      expect(checkDraw(board)).toBe(true)
    })

    it('半满返回 false', () => {
      const board = new Board(15)
      board.placeStone({ row: 7, col: 7 }, 'Black')
      expect(checkDraw(board)).toBe(false)
    })
  })
})