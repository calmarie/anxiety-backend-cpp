# Start the tests via `make test-debug` or `make test-release`

import pytest

from testsuite.databases import pgsql  # noqa: F401


async def test_postgres_ping(service_client):
    response = await service_client.get('/db-ping')
    assert response.status == 200
    assert response.text == 'ok'

