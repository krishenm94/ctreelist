#include "OrdersDockSourceUpdater.h"

#include "ModelStore.h"
#include "OrdersModel.h"

OrdersDockSourceUpdater::OrdersDockSourceUpdater()
{
    const auto &orders = ModelStore::get<OrdersModel>();

    populate(upcast(orders.orders()));

    connect(orders,
            &OrdersModel::added,
            [this](const auto &order)
            {
                ASSERT(order != nullptr, "Order cannot be nullptr")
                if (order != nullptr)
                {
                    add(*order);
                }
            });

    connect(orders,
            &OrdersModel::removed,
            [this](const auto &order)
            {
                ASSERT(order != nullptr, "Order cannot be nullptr")
                if (order != nullptr)
                {
                    remove(*order);
                }
            });

    connect(orders,
            &OrdersModel::cleared,
            [this]()
            {
                clear();
            });
}
