<script setup lang="ts">
import type { SwitchSongSnapshot } from '@/domain/types'

defineProps<{
  snapshot: SwitchSongSnapshot
}>()

const emit = defineEmits<{
  click: []
}>()
</script>

<template>
  <el-button
    :disabled="snapshot.state === 'Transitioning'"
    :loading="snapshot.state === 'Transitioning'"
    size="small"
    class="switch-song-btn"
    @click="emit('click')"
  >
    {{ snapshot.state === 'Transitioning' ? '过渡中' : '换歌' }}
  </el-button>
</template>

<style scoped>
.switch-song-btn {
  width: 100%;
  margin-top: 4px;
}
</style>