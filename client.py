#!/usr/bin/env python

import asyncio
import websockets

uri = "ws://localhost:7681"

async def main():
    async with websockets.connect(uri) as websocket:
            data = bytearray(b'\x00\x0F')
            while True:
                await websocket.send(data)
                await websocket.send(data)
                msg = await websocket.recv()
                print(msg)

asyncio.get_event_loop().run_until_complete(main())
