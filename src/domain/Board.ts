import type { CellState, Player, Position } from './types'

export class Board {
  private readonly _size: number
  private readonly _grid: CellState[][]

  constructor(size: number) {
    Board.validateSize(size)
    this._size = size
    this._grid = Array.from({ length: size }, () =>
      Array.from({ length: size }, () => 'Empty' as CellState)
    )
  }

  static validateSize(size: number): void {
    if (size < 5 || size > 19) {
      throw new Error('棋盘尺寸须为 5 至 19 之间的奇数')
    }
    if (size % 2 === 0) {
      throw new Error('棋盘尺寸须为奇数')
    }
  }

  get size(): number {
    return this._size
  }

  getCell(pos: Position): CellState {
    return this._grid[pos.row - 1][pos.col - 1]
  }

  placeStone(pos: Position, player: Player): void {
    this._grid[pos.row - 1][pos.col - 1] = player === 'Black' ? 'Black' : 'White'
  }

  clearStone(pos: Position): void {
    this._grid[pos.row - 1][pos.col - 1] = 'Empty'
  }

  isEmpty(pos: Position): boolean {
    return this._grid[pos.row - 1][pos.col - 1] === 'Empty'
  }

  isFull(): boolean {
    for (let r = 0; r < this._size; r++) {
      for (let c = 0; c < this._size; c++) {
        if (this._grid[r][c] === 'Empty') return false
      }
    }
    return true
  }

  reset(): void {
    for (let r = 0; r < this._size; r++) {
      for (let c = 0; c < this._size; c++) {
        this._grid[r][c] = 'Empty'
      }
    }
  }

  isValidPosition(pos: Position): boolean {
    return pos.row >= 1 && pos.row <= this._size && pos.col >= 1 && pos.col <= this._size
  }

  clone(): Board {
    const copy = new Board(this._size)
    for (let r = 0; r < this._size; r++) {
      for (let c = 0; c < this._size; c++) {
        copy._grid[r][c] = this._grid[r][c]
      }
    }
    return copy
  }
}