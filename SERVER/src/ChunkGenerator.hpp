/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#include <memory>
#include <thread>
#include <vector>
#include "Generator.hpp"
#include "Chunk.hpp"

struct ChunkGenerator
{
    std::unique_ptr<ChunkDB> generate()
    {
        auto chunk_db = std::make_unique<ChunkDB>();

        static const std::size_t THREADS = std::max(1u, std::thread::hardware_concurrency());

        static const std::size_t CHUNKS_PER_THREAD = CHUNK_DB_SIZE / THREADS;

        auto generate_range = [&chunk_db](std::size_t begin, std::size_t end) mutable
        {
            for (std::size_t i = begin; i < end; i++)
                chunk_db->at(i).generate();
        };

        std::vector<std::thread> threads;

        for (std::size_t i = 0; i < THREADS - 1; i++)
            threads.emplace_back(generate_range, i * CHUNKS_PER_THREAD,
                                                  (i + 1) * CHUNKS_PER_THREAD);

        generate_range((THREADS - 1) * CHUNKS_PER_THREAD, chunk_db->size());

        // wait for all thread to get done
        std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

        return chunk_db;
    }
};
