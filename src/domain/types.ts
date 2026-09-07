export type CellState = 'Empty' | 'Black' | 'White'
export type Player = 'Black' | 'White'
export type GameState = 'Uninitialized' | 'InProgress' | 'BlackWin' | 'WhiteWin' | 'Draw'
export type Direction = 'Horizontal' | 'Vertical' | 'LeftDiagonal' | 'RightDiagonal'

export interface Position {
  row: number
  col: number
}

export interface MoveRecord {
  pos: Position
  player: Player
}

export interface WinningLine {
  direction: Direction
  start: Position
  length: number
  player: Player
}