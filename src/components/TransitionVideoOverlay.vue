<script setup lang="ts">
import { ref, watch, nextTick } from 'vue'
import SkipButton from './SkipButton.vue'

const props = defineProps<{
  visible: boolean
  skipButtonVisible: boolean
}>()

const emit = defineEmits<{
  'container-ready': [el: HTMLElement]
  skip: []
}>()

const containerRef = ref<HTMLDivElement | null>(null)

watch(
  () => props.visible,
  async (visible: boolean) => {
    if (visible) {
      await nextTick()
      if (containerRef.value) {
        emit('container-ready', containerRef.value)
      }
    }
  }
)
</script>

<template>
  <div v-if="visible" class="transition-overlay">
    <div ref="containerRef" class="video-container" />
    <SkipButton :visible="skipButtonVisible" @skip="emit('skip')" />
  </div>
</template>

<style scoped>
.transition-overlay {
  position: fixed;
  inset: 0;
  z-index: 200;
  background: rgba(0, 0, 0, 0.95);
  display: flex;
  align-items: center;
  justify-content: center;
}

.video-container {
  width: 100%;
  height: 100%;
  display: flex;
  align-items: center;
  justify-content: center;
}
</style>
