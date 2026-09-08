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

export type PlayerSeat = 'A' | 'B'
export type Parity = 'Odd' | 'Even'
export type OpeningPhase = 'ColorSelect' | 'GuessFirst' | 'Completed'
export type ColorSelectState = 'Pending' | 'Confirmed' | 'Assigned'
export type GuessFirstState = 'WaitingRoll' | 'Rolled' | 'Guessed' | 'Judged'
export type ColorAssignResult = 'ABlackBWhite' | 'AWhiteBBlack'
export type SeatAssignResult = 'AOneBTwo' | 'ATwoBOne'

export interface OpeningAssignment {
  colorAssign: ColorAssignResult
  seatAssign: SeatAssignResult
}

export type BgmSwitch = 'On' | 'Off'
export type SfxSwitch = 'On' | 'Off'
export type AudioUnlockState = 'Locked' | 'Unlocked'
export type BgmPlayState = 'Playing' | 'Paused'
export type SfxType = 'Place' | 'Win' | 'Draw'

export type SwitchSongState = 'Idle' | 'Transitioning'
export type SwitchSongResult = 'Success' | 'TransitionUnavailable' | 'HandoffFailed'

export interface SwitchSongSnapshot {
  state: SwitchSongState
  lastResult: SwitchSongResult
}
