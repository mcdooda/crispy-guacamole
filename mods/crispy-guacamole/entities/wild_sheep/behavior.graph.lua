return {
  links = {
    {
      1,
      1,
      2,
      1,
    },
    {
      1,
      2,
      3,
      1,
    },
    {
      1,
      3,
      3,
      2,
    },
  },
  nodeType = 'script',
  nodes = {
    {
      name = 'components/behaviorcomponent',
    },
    {
      loadArguments = {
        {
          5,
          'Component',
        },
      },
      name = 'script/scriptoutput',
    },
    {
      loadArguments = {
        1,
        {
          links = {
            {
              1,
              1,
              2,
              2,
            },
            {
              2,
              1,
              2,
              3,
            },
            {
              2,
              2,
              9,
              2,
            },
            {
              2,
              2,
              13,
              2,
            },
            {
              3,
              1,
              2,
              1,
            },
            {
              4,
              1,
              2,
              6,
            },
            {
              5,
              1,
              4,
              1,
            },
            {
              7,
              1,
              2,
              4,
            },
            {
              7,
              2,
              11,
              2,
            },
            {
              8,
              1,
              7,
              1,
            },
            {
              9,
              1,
              7,
              3,
            },
            {
              10,
              1,
              9,
              1,
            },
            {
              11,
              1,
              7,
              2,
            },
            {
              12,
              1,
              11,
              1,
            },
            {
              13,
              1,
              15,
              2,
            },
            {
              14,
              1,
              13,
              1,
            },
            {
              15,
              1,
              2,
              5,
            },
            {
              16,
              1,
              15,
              1,
            },
          },
          nodeType = 'statemachine',
          nodes = {
            {
              name = 'statemachine/enter',
            },
            {
              loadArguments = {
                6,
                1,
                {
                  links = {
                    {
                      1,
                      1,
                      12,
                      1,
                    },
                    {
                      4,
                      1,
                      3,
                      1,
                    },
                    {
                      5,
                      1,
                      3,
                      2,
                    },
                    {
                      12,
                      1,
                      15,
                      1,
                    },
                    {
                      13,
                      1,
                      12,
                      3,
                    },
                    {
                      14,
                      1,
                      12,
                      2,
                    },
                    {
                      14,
                      1,
                      15,
                      2,
                    },
                    {
                      16,
                      1,
                      15,
                      3,
                    },
                  },
                  nodeType = 'script',
                  nodes = {
                    {
                      name = 'script/init',
                    },
                    {
                      name = 'entity/getposition',
                    },
                    {
                      name = 'math/add',
                    },
                    {
                      loadArguments = {
                        0,
                      },
                      name = 'number',
                    },
                    {
                      loadArguments = {
                        0,
                      },
                      name = 'number',
                    },
                    {
                      loadArguments = {
                        0,
                      },
                      name = 'number',
                    },
                    {
                      loadArguments = {
                        0,
                      },
                      name = 'number',
                    },
                    {
                      loadArguments = {
                        false,
                      },
                      name = 'boolean',
                    },
                    {
                      loadArguments = {
                        0,
                      },
                      name = 'number',
                    },
                    {
                      loadArguments = {
                        0,
                      },
                      name = 'number',
                    },
                    {
                      loadArguments = {
                        0,
                      },
                      name = 'number',
                    },
                    {
                      name = 'entity/setmoveanimation',
                    },
                    {
                      loadArguments = {
                        'move',
                      },
                      name = 'string',
                    },
                    {
                      name = 'script/getcontext',
                    },
                    {
                      name = 'entity/setspeed',
                    },
                    {
                      loadArguments = {
                        0.5,
                      },
                      name = 'number',
                    },
                    {
                      loadArguments = {
                        0,
                      },
                      name = 'number',
                    },
                    {
                      loadArguments = {
                        0,
                      },
                      name = 'number',
                    },
                    {
                      loadArguments = {
                        0,
                      },
                      name = 'number',
                    },
                    {
                      loadArguments = {
                        0,
                      },
                      name = 'number',
                    },
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'wander',
              },
              name = 'string',
            },
            {
              loadArguments = {
                1,
                2,
                {
                  links = {
                  },
                  nodeType = 'script',
                  nodes = {
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'flee',
              },
              name = 'string',
            },
            {
              loadArguments = {
                'dummy loop',
              },
              name = 'string',
            },
            {
              loadArguments = {
                3,
                3,
                {
                  links = {
                  },
                  nodeType = 'script',
                  nodes = {
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'graze',
              },
              name = 'string',
            },
            {
              loadArguments = {
                4,
                {
                  links = {
                  },
                  nodeType = 'script',
                  nodes = {
                  },
                },
              },
              name = 'statemachine/rule',
            },
            {
              loadArguments = {
                'graze 30%',
              },
              name = 'string',
            },
            {
              loadArguments = {
                5,
                {
                  links = {
                  },
                  nodeType = 'script',
                  nodes = {
                  },
                },
              },
              name = 'statemachine/rule',
            },
            {
              loadArguments = {
                'loop graze 30%',
              },
              name = 'string',
            },
            {
              loadArguments = {
                6,
                {
                  links = {
                  },
                  nodeType = 'script',
                  nodes = {
                  },
                },
              },
              name = 'statemachine/rule',
            },
            {
              loadArguments = {
                'sleep 10%',
              },
              name = 'string',
            },
            {
              loadArguments = {
                2,
                7,
                {
                  links = {
                  },
                  nodeType = 'script',
                  nodes = {
                  },
                },
              },
              name = 'statemachine/state',
            },
            {
              loadArguments = {
                'sleep',
              },
              name = 'string',
            },
          },
        },
      },
      name = 'statemachine',
    },
  },
}